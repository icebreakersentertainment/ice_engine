#include <iostream>
#include <sstream>
#include <utility>
#include <functional>
#include <algorithm>
#include <chrono>
#include <sstream>

#include <boost/archive/text_oarchive.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "GameEngine.hpp"
#include "Scene.hpp"
#include "BindingDelegate.hpp"
#include "ScriptFunctionHandleWrapper.hpp"

#include "Constants.hpp"

#include "ModelLoader.hpp"
#include "graphics/model/Animate.hpp"

#include "TerrainFactory.hpp"
#include "scripting/ScriptingFactory.hpp"

#include "graphics/Event.hpp"

#include "logger/Logger.hpp"
#include "fs/FileSystem.hpp"
#include "Image.hpp"

#include "resources/EngineResourceManager.MeshHandle.hpp"
#include "resources/EngineResourceManager.TextureHandle.hpp"
#include "resources/EngineResourceManager.VertexShaderHandle.hpp"

#include "exceptions/FileNotFoundException.hpp"
#include "exceptions/InvalidArgumentException.hpp"

#include "utilities/IoUtilities.hpp"

#include "detail/Validation.hpp"

#define CATCH_COPY_AND_RETHROW() \
catch (const ice_engine::Exception& e) \
{ \
    LOG_ERROR(logger_, "Error: %s", boost::diagnostic_information(e)); \
	throw ice_engine::Exception(e); \
} \
catch (const std::exception& e) \
{ \
    LOG_ERROR(logger_, "Error: %s", boost::diagnostic_information(e)); \
	throw ice_engine::Exception(e); \
}

namespace ice_engine
{

GameEngine::GameEngine(
	std::unique_ptr<utilities::Properties> properties,
    std::unique_ptr<fs::IFileSystem> fileSystem,
	std::unique_ptr<ice_engine::IPluginManager> pluginManager,
	std::unique_ptr<ice_engine::logger::ILogger> logger
)
	:
	properties_(std::move(properties)),
	fileSystem_(std::move(fileSystem)),
	pluginManager_(std::move(pluginManager)),
	logger_(std::move(logger))
{
	try
	{
		initialize();
	}
	CATCH_COPY_AND_RETHROW()
}

GameEngine::GameEngine(
	std::unique_ptr<utilities::Properties> properties,
    std::unique_ptr<fs::IFileSystem> fileSystem,
	std::unique_ptr<ice_engine::logger::ILogger> logger,
	std::unique_ptr<ice_engine::IPluginManager> pluginManager,
	std::unique_ptr<graphics::IGraphicsEngineFactory> graphicsEngineFactory,
	std::unique_ptr<ITerrainFactory> terrainFactory
)
	:
	properties_(std::move(properties)),
    fileSystem_(std::move(fileSystem)),
	logger_(std::move(logger)),
	pluginManager_(std::move(pluginManager)),
	graphicsEngineFactory_(std::move(graphicsEngineFactory)),
	terrainFactory_(std::move(terrainFactory))
{
	initialize();
}

GameEngine::~GameEngine()
{
	LOG_INFO(logger_, "Shutting down.");

    if (debuggerExecutionContext_)
    {
        scriptingEngine_->destroyExecutionContext(debuggerExecutionContext_);
    }

    while (!temporaryExecutionContexts_.empty())
    {
        const auto context = temporaryExecutionContexts_.front();

        scriptingEngine_->destroyExecutionContext(context);

        temporaryExecutionContexts_.pop();
    }

	// Make sure the GUIs get deleted before the GUI plugins are deleted
	guis_.clear();

	// Make sure the Modules get deleted before the plugin is unloaded
	modules_.clear();

	// Make sure the Bindings get deleted before the plugin is unloaded
	scriptingEngineBindings_.clear();
}

const EngineStatistics& GameEngine::getEngineStatistics() const
{
	return engineStatistics_;
}

void GameEngine::exit()
{
	running_ = false;
}

scripting::ExecutionContextHandle GameEngine::acquireTemporaryExecutionContext()
{
	scripting::ExecutionContextHandle context;

	std::lock_guard<std::mutex> lockGuard(temporaryExecutionContextMutex_);

	if (temporaryExecutionContexts_.empty())
	{
		throw std::runtime_error("No more temporary execution contexts available.");
	}

	context = scripting::ExecutionContextHandle( temporaryExecutionContexts_.front() );

	temporaryExecutionContexts_.pop();

	return context;
}

void GameEngine::releaseTemporaryExecutionContext(const scripting::ExecutionContextHandle& executionContextHandle)
{
	std::lock_guard<std::mutex> lockGuard(temporaryExecutionContextMutex_);

	temporaryExecutionContexts_.push(executionContextHandle);
}

std::shared_future<void> GameEngine::postWorkToForegroundThreadPool(void* object) {
    scripting::ScriptFunctionHandle scriptFunctionHandle(object);

    auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

    auto promise = std::make_shared<std::promise<void>>();
    auto sharedFuture = promise->get_future().share();

    std::function<void()> work = [=, &logger_ = logger_, &scriptingEngine_ = scriptingEngine_, promise = promise, sharedFuture = sharedFuture, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper]() {
        auto context = this->acquireTemporaryExecutionContext();
        try
        {
            scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), context);
            promise->set_value();
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger_, "Error while executing script function: %s", e.what())
        }

        this->releaseTemporaryExecutionContext(context);
    };

    foregroundThreadPool_->postWork(std::move(work));

    return sharedFuture;
}

std::shared_future<void> GameEngine::postWorkToBackgroundThreadPool(void* object)
{
	scripting::ScriptFunctionHandle scriptFunctionHandle(object);

	auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

	auto promise = std::make_shared<std::promise<void>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> work = [=, &logger_ = logger_, &scriptingEngine_ = scriptingEngine_, promise = promise, sharedFuture = sharedFuture, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper]() {
		auto context = this->acquireTemporaryExecutionContext();
		try
		{
			scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), context);
			promise->set_value();
		}
		catch (const std::exception& e)
		{
			promise->set_exception(std::current_exception());
			LOG_ERROR(logger_, "Error while executing script function: %s", e.what())
		}

		this->releaseTemporaryExecutionContext(context);
	};

	backgroundThreadPool_->postWork(std::move(work));

	return sharedFuture;
}

std::shared_future<void> GameEngine::postWorkToOpenGlWorker(void* object) {
    scripting::ScriptFunctionHandle scriptFunctionHandle(object);

    auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

    auto promise = std::make_shared<std::promise<void>>();
    auto sharedFuture = promise->get_future().share();

    std::function<void()> work = [=, &logger_ = logger_, &scriptingEngine_ = scriptingEngine_, promise = promise, sharedFuture = sharedFuture, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper]() {
        auto context = this->acquireTemporaryExecutionContext();
        try
        {
            scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), context);
            promise->set_value();
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            std::cerr << "An exception occured: " << boost::diagnostic_information(e) << std::endl;
            LOG_ERROR(logger_, "Error while executing script function: %s", e.what())
        }

        this->releaseTemporaryExecutionContext(context);
    };

    openGlLoader_->postWork(std::move(work));

    return sharedFuture;
}

void GameEngine::tick(const float32 delta)
{
	handleEvents();

	scripting::ParameterList params;
	params.add(delta);

	scriptingEngine_->execute(scriptObjectHandle_, "void tick(const float)", params);

	std::vector<std::future<void>> futures;
	for (auto& scene : scenes_)
	{
		std::function<void()> work = [&scene, delta = delta]() {
			scene->tick(delta);
		};
		futures.push_back(foregroundThreadPool_->postWork(std::move(work)));
	}

	for (auto& f : futures)
	{
		f.wait();
		f.get();
	}

	while (foregroundThreadPool_->getWorkQueueCount() + foregroundThreadPool_->getActiveWorkerCount() > 0)
	{
		// sleep
	}

	for (auto& module : modules_)
	{
		module->tick(delta);
	}

	for (auto& gui : guis_)
	{
		gui->tick(delta);
	}

    // We do the creation and destruction asynchronously so that we don't concurrently modify the guis_ vector
    {
        std::lock_guard<std::mutex> lockGuard(guisCreatedMutex_);
        for (auto& gui : guisCreated_)
        {
            guis_.push_back(std::move(gui));
        }

        guisCreated_.clear();
    }

    {
        std::lock_guard<std::mutex> lockGuard(guisDeletedMutex_);
        for (const auto gui : guisDeleted_)
        {
            internalDestroyGui(gui);
        }

        guisDeleted_.clear();
    }

	// Load any opengl assets (this needs work...)
	if (openGlLoader_->getWorkQueueCount() != 0u)
	{
		openGlLoader_->tick();
	}

	while (forgroundGraphicsThreadPool_->getWorkQueueCount() != 0u)
	{
		forgroundGraphicsThreadPool_->tick();
	}
}

void GameEngine::render()
{
    graphicsEngine_->beginRender();

    for (auto& scene : scenes_)
    {
        scene->render();
    }

    debugRenderer_->render();

    for (auto& gui : guis_)
    {
        gui->render();
    }

    graphicsEngine_->endRender();
}

void GameEngine::destroy()
{
}

void GameEngine::initialize()
{
	running_ = true;

	bootstrapScriptName_ = "bootstrap.as";

	initializeLoggingSubSystem();

	LOG_INFO(logger_, "Initializing...");

	initializeFileSystemSubSystem();

	initializeDataStoreSubSystem();

	initializeThreadingSubSystem();

	initializeEntitySubSystem();

	initializeGraphicsSubSystem();

	initializeTerrainSubSystem();

	initializeAudioSubSystem();

	initializeNetworkingSubSystem();

	initializePhysicsSubSystem();

	initializePathfindingSubSystem();

	initializeScriptingSubSystem();

	initializeInputSubSystem();

	initializeModuleSubSystem();

    initializeResourceManagers();

    initializeEngineResourceManagers();

	LOG_INFO(logger_, "Done initialization.");
}

void GameEngine::initializeLoggingSubSystem()
{
	// Initialize the log using the specified log file
	if (!logger_)
	{
		logger_ = std::make_unique<logger::Logger>("ice_engine.log");
	}
}

void GameEngine::initializeFileSystemSubSystem()
{
	LOG_INFO(logger_, "initialize file system.");

    if (!fileSystem_)
    {
        fileSystem_ = std::make_unique<fs::FileSystem>();
    }
}

void GameEngine::initializeInputSubSystem()
{
	LOG_INFO(logger_, "initialize keyboard and mouse.");
}

void GameEngine::initializePhysicsSubSystem()
{
	LOG_INFO(logger_, "initializing physics.");

	if (!physicsEngineFactory_)
	{
        const auto physicsPlugin = pluginManager_->getPhysicsPlugin();

		if (physicsPlugin)
		{
			LOG_INFO(logger_, "initializing physics plugin %s.", physicsPlugin->getName());
			physicsEngineFactory_ = physicsPlugin->createFactory();

			physicsEngine_ = physicsEngineFactory_->create(properties_.get(), fileSystem_.get(), logger_.get());

			physicsEngine_->setPhysicsDebugRenderer(debugRenderer_.get());
		}
		else
		{
			LOG_INFO(logger_, "physics system not initialized - no physics plugin found.");
//			physicsEngineFactory_ = std::make_unique<physics::PhysicsEngineFactory>();
		}
	}
}

void GameEngine::initializePathfindingSubSystem()
{
	LOG_INFO(logger_, "initializing pathfinding.");

	if (!pathfindingEngineFactory_)
	{
        const auto pathfindingPlugin = pluginManager_->getPathfindingPlugin();

		if (pathfindingPlugin)
		{
			LOG_INFO(logger_, "initializing pathfinding plugin %s.", pathfindingPlugin->getName());
			pathfindingEngineFactory_ = pathfindingPlugin->createFactory();

			pathfindingEngine_ = pathfindingEngineFactory_->create(properties_.get(), fileSystem_.get(), logger_.get());

			pathfindingEngine_->setPathfindingDebugRenderer(debugRenderer_.get());
		}
		else
		{
			LOG_INFO(logger_, "pathfinding system not initialized - no pathfinding plugin found.");
//			pathfindingEngineFactory_ = std::make_unique<pathfinding::PathfindingEngineFactory>();
		}
	}
}

void GameEngine::initializeGraphicsSubSystem()
{
	LOG_INFO(logger_, "initializing graphics.");

	if (!graphicsEngineFactory_)
	{
        const auto graphicsPlugin = pluginManager_->getGraphicsPlugin();

		if (graphicsPlugin)
		{
			LOG_INFO(logger_, "initializing graphics plugin %s.", graphicsPlugin->getName());
			graphicsEngineFactory_ = graphicsPlugin->createFactory();

			graphicsEngine_ = graphicsEngineFactory_->create(properties_.get(), fileSystem_.get(), logger_.get());

			graphicsEngine_->addEventListener(this);

			debugRenderer_ = std::make_unique<DebugRenderer>(graphicsEngine_.get());
		}
		else
		{
			LOG_INFO(logger_, "graphics system not initialized - no graphics plugin found.");
//			graphicsEngineFactory_ = std::make_unique<graphics::GraphicsEngineFactory>();
		}
	}
}

void GameEngine::initializeTerrainSubSystem()
{
	LOG_INFO(logger_, "initializing terrain.");

	if (!terrainFactory_)
	{
		terrainFactory_ = std::make_unique<TerrainFactory>();
	}

	//terrainManager_ = terrainFactory_->create( properties_.get(), fileSystem_.get(), logger_.get(), graphicsEngine_.get(), pathfindingEngine_.get(), scriptingEngine_.get(), audioEngine_.get(), networkingEngine_.get() );
}

void GameEngine::initializeModuleSubSystem()
{
	LOG_INFO(logger_, "initializing modules.");

	const auto& modulePlugins = pluginManager_->getModulePlugins();

	for (auto modulePlugin : modulePlugins)
	{
		auto module = modulePlugin->createFactory()->create(properties_.get(), fileSystem_.get(), logger_.get(), graphicsEngine_.get(), pathfindingEngine_.get(), scriptingEngine_.get(), audioEngine_.get(), networkingEngine_.get());
		modules_.push_back(std::move(module));
	}
}

void GameEngine::initializeResourceManagers()
{
    LOG_INFO(logger_, "initializing resource managers.");

    LOG_INFO(logger_, "initializing resource importers.");

    LOG_INFO(logger_, "initializing image resource importers.");

    std::unordered_map<std::string, std::unique_ptr<IResourceImporter<Image>>> imageResourceImporters;

    const auto& imageResourceImporterPlugins = pluginManager_->getImageResourceImporterPlugins();

    for (const auto imageResourceImporterPlugin : imageResourceImporterPlugins)
    {
        LOG_INFO(logger_, "initializing image resource importer '%s'.", imageResourceImporterPlugin->getName());

        auto imageResourceImporter = imageResourceImporterPlugin->createFactory()->create(properties_.get(), fileSystem_.get(), logger_.get());
        imageResourceImporters[imageResourceImporterPlugin->getName()] = std::move(imageResourceImporter);
    }

    resourceManagers_[typeid(Image)] = std::make_unique<ResourceManager<Image>>(std::move(imageResourceImporters));
}

void GameEngine::initializeEngineResourceManagers()
{
    LOG_INFO(logger_, "initializing engine resource managers.");

    engineResourceManagers_[typeid(graphics::MeshHandle)] = std::make_unique<EngineResourceManager<graphics::MeshHandle>>(graphicsEngine_.get());
    engineResourceManagers_[typeid(graphics::TextureHandle)] = std::make_unique<EngineResourceManager<graphics::TextureHandle>>(graphicsEngine_.get());
    engineResourceManagers_[typeid(graphics::VertexShaderHandle)] = std::make_unique<EngineResourceManager<graphics::VertexShaderHandle>>(graphicsEngine_.get());
}

void GameEngine::initializeAudioSubSystem()
{
	LOG_INFO(logger_, "initializing audio.");

	if (!audioEngineFactory_)
	{
        const auto audioPlugin = pluginManager_->getAudioPlugin();

		if (audioPlugin)
		{
			LOG_INFO(logger_, "initializing audio plugin %s.", audioPlugin->getName());
			audioEngineFactory_ = audioPlugin->createFactory();

			audioEngine_ = audioEngineFactory_->create(properties_.get(), fileSystem_.get(), logger_.get());
		}
		else
		{
			LOG_INFO(logger_, "audio system not initialized - no audio plugin found.");
//			audioEngineFactory_ = std::make_unique<audio::AudioEngineFactory>();
		}
	}
}

void GameEngine::initializeNetworkingSubSystem()
{
	LOG_INFO(logger_, "initializing networking.");

	if (!networkingEngineFactory_)
	{
        const auto networkingPlugin = pluginManager_->getNetworkingPlugin();

		if (networkingPlugin)
		{
			LOG_INFO(logger_, "initializing networking plugin %s.", networkingPlugin->getName());
			networkingEngineFactory_ = networkingPlugin->createFactory();

			networkingEngine_ = networkingEngineFactory_->create(properties_.get(), fileSystem_.get(), logger_.get());

			networkingEngine_->addEventListener(this);
		}
		else
		{
			LOG_INFO(logger_, "networking system not initialized - no networking plugin found.");
//			networkingEngineFactory_ = std::make_unique<networking::NetworkingEngineFactory>();
		}
	}
}

void GameEngine::initializeScriptingSubSystem()
{
	LOG_INFO(logger_, "Initializing scripting...");

	scriptingEngine_ = scripting::ScriptingFactory::createScriptingEngine(properties_.get(), fileSystem_.get(), logger_.get());

	BindingDelegate delegate(logger_.get(), scriptingEngine_.get(), this, graphicsEngine_.get(), audioEngine_.get(), networkingEngine_.get(), physicsEngine_.get(), pathfindingEngine_.get());
	delegate.bind();

    const auto scriptingEngineBindingPlugins = pluginManager_->scriptingEngineBindingPlugins();

	for (auto scriptingEngineBindingPlugin : scriptingEngineBindingPlugins)
	{
		LOG_INFO(logger_, "initializing scripting engine binding plugin %s.", scriptingEngineBindingPlugin->getName());
		auto scriptingEngineBindingFactory = scriptingEngineBindingPlugin->createFactory();

		auto scriptingEngineBinding = scriptingEngineBindingFactory->create(properties_.get(), fileSystem_.get(), logger_.get(), graphicsEngine_.get(), pathfindingEngine_.get(), physicsEngine_.get(), scriptingEngine_.get(), audioEngine_.get(), networkingEngine_.get(), this);
		scriptingEngineBinding->bind();
		scriptingEngineBindings_.push_back(std::move(scriptingEngineBinding));
	}

	scriptingEngine_->debugger()->addDebugEventListener(this);

    debuggerExecutionContext_ = scriptingEngine_->createExecutionContext();

	for (int i=0; i < 2; ++i)
	{
		auto context = scriptingEngine_->createExecutionContext();
		temporaryExecutionContexts_.push(context);
	}
}

void GameEngine::initializeThreadingSubSystem()
{
	LOG_INFO(logger_, "Load thread pool...");
	backgroundThreadPool_ = std::make_unique<ThreadPool>();
	foregroundThreadPool_ = std::make_unique<ThreadPool>();

	LOG_DEBUG(logger_, "Load opengl loader...");
	openGlLoader_ = std::make_unique<OpenGlLoader>();
	forgroundGraphicsThreadPool_ = std::make_unique<OpenGlLoader>();
}

void GameEngine::initializeDataStoreSubSystem()
{
	LOG_INFO(logger_, "Load data store...");
	//dataStore_ = std::unique_ptr<pyliteserializer::SqliteDataStore>( new pyliteserializer::SqliteDataStore(std::string("../data/dark_horizon.db")) );
}

void GameEngine::initializeEntitySubSystem()
{
	LOG_INFO(logger_, "Load entity system...");
}

std::vector<graphics::model::BoneData> boneData;
std::vector<graphics::model::Animation> animations;
graphics::model::BoneNode rootBoneNode;
glm::mat4 globalInverseTransformation = glm::mat4(1.0f);
graphics::SkeletonHandle skeletonHandle;
void GameEngine::test()
{

}

void GameEngine::processEvent(const scripting::DebugEvent& event)
{
    auto debugger = scriptingEngine_->debugger();

    switch (event.type)
    {
        case scripting::HIT_BREAKPOINT:
            {
                LOG_DEBUG(logger_, "Processing breakpoint in file '%s' line %s.", debugger->filename(), debugger->line())
                std::cout << "HIT_BREAKPOINT"
                << " | " << debugger->filename()
                << " | " << debugger->line()
                << std::endl;

                auto beginFpsTime = std::chrono::high_resolution_clock::now();
                auto endFpsTime = std::chrono::high_resolution_clock::now();
                auto previousFpsTime = beginFpsTime;
                float32 currentFps = 0.0f;
                float32 tempFps = 0.0f;
                float32 delta = 0.0f;
                float32 cumulativeDelta = 0.0f;

//                graphicsEngine_->setWindowGrab(false);
                const bool cursorVisible = graphicsEngine_->cursorVisible();
                graphicsEngine_->setCursorVisible(true);

                while (debugger->running())
                {
                    beginFpsTime = std::chrono::high_resolution_clock::now();
                    delta = std::chrono::duration<float32>(beginFpsTime - endFpsTime).count();
                    cumulativeDelta += delta;

                    if (std::chrono::duration<float32>(beginFpsTime - previousFpsTime).count() > 1.0f)
                    {
                        previousFpsTime = beginFpsTime;
                        currentFps = tempFps;
                        tempFps = 0;
                    }

                    const float32 updateFrequency = 1.0f / 60.0f;
                    int a = 0;
                    while (cumulativeDelta >= updateFrequency && a < 3)
                    {
                        for (auto handler : scriptingEngineDebugHandlers_)
                        {
                            handler->tick(delta);
                        }

                        for (const auto& data : scriptScriptingEngineDebugHandlers_)
                        {
                            scripting::ParameterList arguments;
                            arguments.add(delta);

//                            debugger->setEnabled(false);
                            scriptingEngine_->execute(data.first, data.second, arguments, debuggerExecutionContext_);
//                            debugger->setEnabled(true);
                        }

                        handleEvents();

                        for (auto& gui : guis_)
                        {
                            gui->tick(delta);
                        }

                        cumulativeDelta -= updateFrequency;
                        a++;
                    }

                    render();

                    endFpsTime = beginFpsTime;
                }

                std::cout << "DONE " << std::endl;

                graphicsEngine_->setCursorVisible(cursorVisible);
            }
            break;

        case scripting::DONE:

            break;

        default:
            break;
    }
}

Scene* GameEngine::createScene(const std::string& name, const std::vector<std::string>& scriptData, const std::string& initializationFunctionName)
{
	LOG_DEBUG(logger_, "Create Scene with name '%s'", name);

	auto scene = std::make_unique<Scene>(name, scriptData, initializationFunctionName, this, terrainFactory_.get(), properties_.get(), fileSystem_.get(), logger_.get());

    internalInitializeScene(scene);

	scenes_.push_back(std::move(scene));

	return scenes_.back().get();
}

Scene* GameEngine::createScene(const std::string& name, const scripting::ModuleHandle moduleHandle, const std::string& initializationFunctionName)
{
	LOG_DEBUG(logger_, "Create Scene with name '%s' and module handle '%s'", name, moduleHandle);

	auto scene = std::make_unique<Scene>(name, moduleHandle, initializationFunctionName, this, terrainFactory_.get(), properties_.get(), fileSystem_.get(), logger_.get());

    internalInitializeScene(scene);

	scenes_.push_back(std::move(scene));

	return scenes_.back().get();
}

void GameEngine::internalInitializeScene(std::unique_ptr<Scene>& scene)
{
    for (auto callback : preSerializeCallbacks_)
    {
        scene->addPreSerializeCallback(callback);
    }

    for (auto callback : postSerializeCallbacks_)
    {
        scene->addPostSerializeCallback(callback);
    }

    for (auto callback : preDeserializeCallbacks_)
    {
        scene->addPreDeserializeCallback(callback);
    }

    for (auto callback : postDeserializeCallbacks_)
    {
        scene->addPostDeserializeCallback(callback);
    }
}

void GameEngine::destroyScene(const std::string& name)
{
	const auto scene = getScene(name);

	if (scene != nullptr)
	{
		destroyScene(scene);
	}
}

void GameEngine::destroyScene(const Scene* scene)
{
	auto func = [scene](const std::unique_ptr<Scene>& s) {
		return s.get() == scene;
	};

	const auto it = std::find_if(scenes_.begin(), scenes_.end(), func);

	if (it != scenes_.end())
	{
		scenes_.erase(it);
	}
}

Scene* GameEngine::getScene(const std::string& name) const
{
	auto func = [&name](const std::unique_ptr<Scene>& s) {
		return s->name() == name;
	};

	auto it = std::find_if(scenes_.begin(), scenes_.end(), func);

	if (it != scenes_.end())
	{
		return it->get();
	}

	return nullptr;
}

std::vector<Scene*> GameEngine::getAllScenes() const
{
	std::vector<Scene*> scenes;

	for (auto& scene : scenes_)
	{
		scenes.push_back(scene.get());
	}

	return scenes;
}

void GameEngine::testSerialize()
{
	for (auto& scene : scenes_)
	{
		scene->serialize("test.out");
	}
}

void GameEngine::setIGameInstance(void* object)
{
	scriptObjectHandle_ = scripting::ScriptObjectHandle(object);
}

void GameEngine::setBootstrapScript(const std::string& filename)
{
	bootstrapScriptName_ = filename;
	/*
	// Error check
	if (mainAsScript_ != nullptr)
	{
		// TODO: Throw exception
		assert(0);
	}

	scriptingEngine_->loadScript(std::string("Main"), filename);

	mainAsScript_ = scriptingEngine_->createAsObject(std::string("Main"), className);

	mainAsScript_->callMethod( std::string("void initialize()") );
	*/
}

audio::IAudioEngine* GameEngine::audioEngine() const
{
	return audioEngine_.get();
}

graphics::IGraphicsEngine* GameEngine::graphicsEngine() const
{
	return graphicsEngine_.get();
}

physics::IPhysicsEngine* GameEngine::physicsEngine() const
{
	return physicsEngine_.get();
}

scripting::IScriptingEngine* GameEngine::scriptingEngine() const
{
	return scriptingEngine_.get();
}

IDebugRenderer* GameEngine::debugRenderer() const
{
	return debugRenderer_.get();
}

pathfinding::IPathfindingEngine* GameEngine::pathfindingEngine() const
{
	return pathfindingEngine_.get();
}

IThreadPool* GameEngine::backgroundThreadPool() const
{
	return backgroundThreadPool_.get();
}

IThreadPool* GameEngine::foregroundThreadPool() const
{
	return foregroundThreadPool_.get();
}

IOpenGlLoader* GameEngine::openGlLoader() const
{
	return openGlLoader_.get();
}

logger::ILogger* GameEngine::logger() const
{
	return logger_.get();
}

fs::IFileSystem* GameEngine::fileSystem() const
{
	return fileSystem_.get();
}

graphics::gui::IGui* GameEngine::createGui(const std::string& name)
{
	const auto& guiPlugins = pluginManager_->getGuiPlugins();

	auto guiPlugin = std::find_if(guiPlugins.begin(), guiPlugins.end(), [&name](const auto& guiPlugin) -> bool { return guiPlugin->getName() == name; });

	if (guiPlugin == guiPlugins.end())
	{
		throw std::runtime_error(detail::format("Gui plugin with name '%s' does not exist.", name));
	}

	auto gui = (*guiPlugin)->createFactory()->create(properties_.get(), fileSystem_.get(), logger_.get(), graphicsEngine_.get());
    auto ptr = gui.get();

    std::lock_guard<std::mutex> lockGuard(guisCreatedMutex_);
    guisCreated_.push_back(std::move(gui));

	return ptr;
}

void GameEngine::destroyGui(const graphics::gui::IGui* gui)
{
    std::lock_guard<std::mutex> lockGuard(guisDeletedMutex_);
    guisDeleted_.push_back(gui);
}

void GameEngine::internalDestroyGui(const graphics::gui::IGui* gui)
{
	auto it = std::find_if(guis_.begin(), guis_.end(), [gui](const auto& guiUniquePtr) -> bool { return guiUniquePtr.get() == gui; });
	if (it != guis_.end())
	{
		guis_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy gui - gui was not found.");
	}
}

void GameEngine::setCallback(graphics::gui::IButton* button, void* object)
{
	scripting::ScriptFunctionHandle scriptFunctionHandle(object);

	auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

	std::function<void (void)> func = [&scriptingEngine_ = scriptingEngine_, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper]() {
		scriptingEngine_->execute(scriptFunctionHandleWrapper.get());
	};

	button->setCallback(func);
}

void GameEngine::setOnChangeCallback(graphics::gui::ITextField* textField, void* object)
{
    scripting::ScriptFunctionHandle scriptFunctionHandle(object);

    auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

    std::function<void (void)> func = [&scriptingEngine_ = scriptingEngine_, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper]() {
        scriptingEngine_->execute(scriptFunctionHandleWrapper.get());
    };

    textField->setOnChangeCallback(func);
}

void GameEngine::setOnChangeCallback(graphics::gui::ITextArea* textArea, void* object)
{
    scripting::ScriptFunctionHandle scriptFunctionHandle(object);

    auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

    std::function<void (void)> func = [&scriptingEngine_ = scriptingEngine_, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper]() {
        scriptingEngine_->execute(scriptFunctionHandleWrapper.get());
    };

    textArea->setOnChangeCallback(func);
}

void GameEngine::setCallback(graphics::gui::IMenuItem* menuItem, void* object)
{
	scripting::ScriptFunctionHandle scriptFunctionHandle(object);

	auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

	std::function<void (void)> func = [&scriptingEngine_ = scriptingEngine_, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper]() {
		scriptingEngine_->execute(scriptFunctionHandleWrapper.get());
	};

	menuItem->setCallback(func);
}

void GameEngine::setCallback(graphics::gui::IComboBox* comboBox, void* object)
{
	scripting::ScriptFunctionHandle scriptFunctionHandle(object);

	auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

	std::function<void (graphics::gui::IComboBoxItem*)> func = [&scriptingEngine_ = scriptingEngine_, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper](graphics::gui::IComboBoxItem* comboBoxItem) {
        scripting::ParameterList arguments;
        arguments.addRef(*comboBoxItem);

		scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), arguments);
	};

    comboBox->setCallback(func);
}

void GameEngine::setCallback(graphics::gui::ITreeView* treeView, void* object)
{
	scripting::ScriptFunctionHandle scriptFunctionHandle(object);

	auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

	std::function<void (graphics::gui::ITreeNode*)> func = [&scriptingEngine_ = scriptingEngine_, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper](graphics::gui::ITreeNode* treeNode) {
        scripting::ParameterList arguments;
        arguments.addRef(*treeNode);

		scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), arguments);
	};

    treeView->setCallback(func);
}

Audio* GameEngine::loadAudio(const std::string& name, const std::string& filename)
{
	LOG_DEBUG(logger_, "Loading audio: %s", filename);
	if (!fileSystem_->exists(filename))
	{
		throw std::runtime_error(detail::format("Audio file '%s' does not exist.", filename));
	}

	auto file = fileSystem_->open(filename, fs::FileFlags::READ | fs::FileFlags::BINARY);
	resourceCache_.addAudio(name, std::make_unique<Audio>(file->getInputStream()));

	LOG_DEBUG(logger_, "Done loading audio: %s", filename);

	return resourceCache_.getAudio(name);
}

std::shared_future<Audio*> GameEngine::loadAudioAsync(const std::string& name, const std::string& filename)
{
	auto promise = std::make_shared<std::promise<Audio*>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, &logger = logger_, promise = promise, sharedFuture = sharedFuture, name = name, filename = filename]() {
        try
        {
            auto audio = this->loadAudio(name, filename);
            promise->set_value(audio);
        }
        catch (const Exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while importing audio '%s' with filename '%s': %s", name, filename, boost::diagnostic_information(e));
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while importing audio '%s' with filename '%s': %s", name, filename, boost::diagnostic_information(e));
        }

	};

	backgroundThreadPool_->postWork(std::move(func));

	return sharedFuture;
}

IImage* GameEngine::loadImage(const std::string& name, const std::string& filename)
{
	LOG_DEBUG(logger_, "Loading image: %s", filename);
	if (!fileSystem_->exists(filename))
	{
		throw std::runtime_error(detail::format("Image file '%s' does not exist.", filename));
	}

	auto& resourceManager = this->resourceManager<Image>();
    auto test = std::unique_ptr<Image>(resourceManager.import(name, filename));

//	auto file = fileSystem_->open(filename, fs::FileFlags::READ | fs::FileFlags::BINARY);
	resourceCache_.addImage(name, std::move(test));

	LOG_DEBUG(logger_, "Done loading image: %s", filename);

	return resourceCache_.getImage(name);
}

std::shared_future<IImage*> GameEngine::loadImageAsync(const std::string& name, const std::string& filename)
{
	auto promise = std::make_shared<std::promise<IImage*>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, &logger = logger_, promise = promise, sharedFuture = sharedFuture, name = name, filename = filename]() {
        try
        {
            auto image = this->loadImage(name, filename);
            promise->set_value(image);
        }
        catch (const Exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while importing image '%s' with filename '%s': %s", name, filename, boost::diagnostic_information(e));
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while importing image '%s' with filename '%s': %s", name, filename, boost::diagnostic_information(e));
        }
	};

	backgroundThreadPool_->postWork(std::move(func));

	return sharedFuture;
}

Model* GameEngine::loadModel(const std::string& name, const std::string& filename)
{
	throw std::logic_error("loadModel not yet implemented.");
}

std::shared_future<Model*> GameEngine::loadModelAsync(const std::string& name, const std::string& filename)
{
	throw std::logic_error("loadModelAsync not yet implemented.");
}

Model* GameEngine::importModel(const std::string& name, const std::string& filename)
{
	if (!fileSystem_->exists(filename))
	{
		throw FileNotFoundException(detail::format("Model file '%s' does not exist.", filename));
	}

	resourceCache_.addModel(name, std::make_unique<Model>(filename, &resourceCache_, logger_.get(), fileSystem_.get()));

	return resourceCache_.getModel(name);
}

std::shared_future<Model*> GameEngine::importModelAsync(const std::string& name, const std::string& filename)
{
	auto promise = std::make_shared<std::promise<Model*>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, &logger = logger_, promise = promise, sharedFuture = sharedFuture, name = name, filename = filename]() {
		try
		{
			auto model = this->importModel(name, filename);
			promise->set_value(model);
		}
		catch (const Exception& e)
		{
			promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while importing model '%s' with filename '%s': %s", name, filename, boost::diagnostic_information(e));
		}
		catch (const std::exception& e)
		{
			promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while importing model '%s' with filename '%s': %s", name, filename, boost::diagnostic_information(e));
		}
	};

	backgroundThreadPool_->postWork(std::move(func));

	return sharedFuture;
}

void GameEngine::unloadAudio(const std::string& name)
{
	return resourceCache_.removeAudio(name);
}

void GameEngine::unloadImage(const std::string& name)
{
	return resourceCache_.removeImage(name);
}

void GameEngine::unloadModel(const std::string& name)
{
	return resourceCache_.removeModel(name);
}

Audio* GameEngine::getAudio(const std::string& name) const
{
	return resourceCache_.getAudio(name);
}

IImage* GameEngine::getImage(const std::string& name) const
{
	return resourceCache_.getImage(name);
}

Model* GameEngine::getModel(const std::string& name) const
{
	return resourceCache_.getModel(name);
}

ModelHandle GameEngine::loadStaticModel(const Model* model)
{
//	auto meshHandle = graphicsEngine_->createStaticMesh(model->meshes[0].vertices, model->meshes[0].indices, model->meshes[0].colors, model->meshes[0].normals, model->meshes[0].textureCoordinates);
//	auto textureHandle = graphicsEngine_->createTexture2d( static_cast<const graphics::IImage*>(&model->textures[0].image) );
//
//	staticModels_.push_back(std::make_pair(meshHandle, textureHandle));
//
//	auto index = staticModels_.size() - 1;
//
	return ModelHandle();
}

std::shared_future<ModelHandle> GameEngine::loadStaticModelAsync(const Model* model)
{
	auto promise = std::make_shared<std::promise<ModelHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, &logger = logger_, promise = promise, sharedFuture = sharedFuture, model = model]() {
        try
        {
            auto modelHandle = this->loadStaticModel(model);
            promise->set_value(modelHandle);
        }
        catch (const Exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while importing model: %s", boost::diagnostic_information(e));
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while importing model: %s", boost::diagnostic_information(e));
        }
	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::VertexShaderHandle GameEngine::createVertexShader(const std::string& name, const std::string& filename)
{
	LOG_DEBUG(logger_, "Loading vertex shader '%s': %s", name, filename);

	const auto shaderSource = fileSystem_->readAll(filename);

	return createVertexShaderFromSource(name, shaderSource);
}

std::shared_future<graphics::VertexShaderHandle> GameEngine::createVertexShaderAsync(const std::string& name, const std::string& filename)
{
	auto promise = std::make_shared<std::promise<graphics::VertexShaderHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, &logger = logger_, promise = promise, sharedFuture = sharedFuture, name = name, filename = filename]() {
        try
        {
            auto shaderHandle = this->createVertexShader(name, filename);
            promise->set_value(shaderHandle);
        }
        catch (const Exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating vertex shader: %s", boost::diagnostic_information(e));
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating vertex shader: %s", boost::diagnostic_information(e));
        }

	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::VertexShaderHandle GameEngine::createVertexShaderFromSource(const std::string& name, const std::string& data)
{
	LOG_DEBUG(logger_, "Creating vertex shader from source: %s", data);

	if (graphicsEngine_->valid(getVertexShader(name)))
	{
		throw std::runtime_error(detail::format("Vertex shader with name '%s' already exists.", name));
	}

	auto handle = graphicsEngine_->createVertexShader(data);

	vertexShaderHandles_[name] = handle;

	return handle;
}

std::shared_future<graphics::VertexShaderHandle> GameEngine::createVertexShaderFromSourceAsync(const std::string& name, const std::string& data)
{
	auto promise = std::make_shared<std::promise<graphics::VertexShaderHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, &logger = logger_, promise = promise, sharedFuture = sharedFuture, name = name, data = data]() {
        try
        {
            auto shaderHandle = this->createVertexShaderFromSource(name, data);
            promise->set_value(shaderHandle);
        }
        catch (const Exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating vertex shader from source: %s", boost::diagnostic_information(e));
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating vertex shader from source: %s", boost::diagnostic_information(e));
        }
	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::FragmentShaderHandle GameEngine::createFragmentShader(const std::string& name, const std::string& filename)
{
	LOG_DEBUG(logger_, "Loading fragment shader '%s': %s", name, filename);

	const auto shaderSource = fileSystem_->readAll(filename);

	return createFragmentShaderFromSource(name, shaderSource);
}

std::shared_future<graphics::FragmentShaderHandle> GameEngine::createFragmentShaderAsync(const std::string& name, const std::string& filename)
{
	auto promise = std::make_shared<std::promise<graphics::FragmentShaderHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, &logger = logger_, promise = promise, sharedFuture = sharedFuture, name = name, filename = filename]() {
        try
        {
            auto shaderHandle = this->createFragmentShader(name, filename);
            promise->set_value(shaderHandle);
        }
        catch (const Exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating fragment shader: %s", boost::diagnostic_information(e));
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating fragment shader: %s", boost::diagnostic_information(e));
        }
	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::FragmentShaderHandle GameEngine::createFragmentShaderFromSource(const std::string& name, const std::string& data)
{
	LOG_DEBUG(logger_, "Creating fragment shader from source: %s", data);

	if (graphicsEngine_->valid(getFragmentShader(name)))
	{
		throw std::runtime_error(detail::format("Fragment shader with name '%s' already exists.", name));
	}

	auto handle = graphicsEngine_->createFragmentShader(data);

	fragmentShaderHandles_[name] = handle;

	return handle;
}

std::shared_future<graphics::FragmentShaderHandle> GameEngine::createFragmentShaderFromSourceAsync(const std::string& name, const std::string& data)
{
	auto promise = std::make_shared<std::promise<graphics::FragmentShaderHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, &logger = logger_, promise = promise, sharedFuture = sharedFuture, name = name, data = data]() {
        try
        {
            auto shaderHandle = this->createFragmentShaderFromSource(name, data);
            promise->set_value(shaderHandle);
        }
        catch (const Exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating fragment shader from source: %s", boost::diagnostic_information(e));
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating fragment shader from source: %s", boost::diagnostic_information(e));
        }
	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::VertexShaderHandle GameEngine::getVertexShader(const std::string& name) const
{
	auto it = vertexShaderHandles_.find(name);
	if (it != vertexShaderHandles_.end())
	{
		return it->second;
	}

	return graphics::VertexShaderHandle();
}

graphics::FragmentShaderHandle GameEngine::getFragmentShader(const std::string& name) const
{
	auto it = fragmentShaderHandles_.find(name);
	if (it != fragmentShaderHandles_.end())
	{
		return it->second;
	}

	return graphics::FragmentShaderHandle();
}

void GameEngine::destroyShader(const std::string& name)
{
	LOG_DEBUG(logger_, "Destroying shader '%s'", name);
	/*
	auto it = shaderHandles_.find(name);
	if (it != shaderHandles_.end())
	{
		graphicsEngine_->destroy(it->second);
		shaderHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy shader '%s' - shader was not found.", name);
	}
	*/
}

void GameEngine::destroyShader(const graphics::VertexShaderHandle& shaderHandle)
{
	LOG_DEBUG(logger_, "Destroying vertex shader with id '%s'", std::to_string(shaderHandle.id()));

	auto func = [&shaderHandle](const auto& pair) {
		return pair.second == shaderHandle;
	};

    const auto it = std::find_if(vertexShaderHandles_.begin(), vertexShaderHandles_.end(), func);

	if (it != vertexShaderHandles_.end())
	{
        graphicsEngine_->destroy(it->second);
		vertexShaderHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy vertex shader with id '%s' - shader was not found.", shaderHandle.id());
	}
}

void GameEngine::destroyShader(const graphics::FragmentShaderHandle& shaderHandle)
{
	LOG_DEBUG(logger_, "Destroying fragment shader with id '%s'", std::to_string(shaderHandle.id()));

	auto func = [&shaderHandle](const auto& pair) {
		return pair.second == shaderHandle;
	};

    const auto it = std::find_if(fragmentShaderHandles_.begin(), fragmentShaderHandles_.end(), func);

	if (it != fragmentShaderHandles_.end())
	{
        graphicsEngine_->destroy(it->second);
		fragmentShaderHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy fragment shader with id '%s' - shader was not found.", shaderHandle.id());
	}
}


graphics::ShaderProgramHandle GameEngine::createShaderProgram(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle)
{
	LOG_DEBUG(logger_, "Creating shader program '%s' from vertex id '%s' and fragment id '%s'", name, vertexShaderHandle.id(), fragmentShaderHandle.id());

	if (graphicsEngine_->valid(getShaderProgram(name)))
	{
		throw std::runtime_error(detail::format("Shader program with name '%s' already exists.", name));
	}

	auto handle = graphicsEngine_->createShaderProgram(vertexShaderHandle, fragmentShaderHandle);

	shaderProgramHandles_[name] = handle;

	return handle;
}

std::shared_future<graphics::ShaderProgramHandle> GameEngine::createShaderProgramAsync(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle)
{
	auto promise = std::make_shared<std::promise<graphics::ShaderProgramHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, &logger = logger_, promise = promise, sharedFuture = sharedFuture, name = name, vertexShaderHandle = vertexShaderHandle, fragmentShaderHandle = fragmentShaderHandle]() {
        try
        {
            auto shaderProgramHandle = this->createShaderProgram(name, vertexShaderHandle, fragmentShaderHandle);
            promise->set_value(shaderProgramHandle);
        }
        catch (const Exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating shader program: %s", boost::diagnostic_information(e));
        }
        catch (const std::exception& e)
        {
            promise->set_exception(std::current_exception());
            LOG_ERROR(logger, "Error while creating shader program: %s", boost::diagnostic_information(e));
        }
	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::ShaderProgramHandle GameEngine::getShaderProgram(const std::string& name) const
{
	auto it = shaderProgramHandles_.find(name);
	if (it != shaderProgramHandles_.end())
	{
		return it->second;
	}

	return graphics::ShaderProgramHandle();
}

void GameEngine::destroyShaderProgram(const std::string& name)
{
	LOG_DEBUG(logger_, "Destroying shader program '%s'", name);

	auto it = shaderProgramHandles_.find(name);
	if (it != shaderProgramHandles_.end())
	{
        graphicsEngine_->destroy(it->second);
		shaderProgramHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy shader program '%s' - shader program was not found.", name);
	}
}

void GameEngine::destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle)
{
	LOG_DEBUG(logger_, "Destroying shader program with id '%s'", shaderProgramHandle.id());

	auto func = [&shaderProgramHandle](const auto& pair) {
		return pair.second == shaderProgramHandle;
	};

    const auto it = std::find_if(shaderProgramHandles_.begin(), shaderProgramHandles_.end(), func);

	if (it != shaderProgramHandles_.end())
	{
        graphicsEngine_->destroy(it->second);
		shaderProgramHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy shader program with id '%s' - shader program was not found.", shaderProgramHandle.id());
	}
}

graphics::RenderableHandle GameEngine::createRenderable(
	const graphics::RenderSceneHandle& renderSceneHandle,
	const ModelHandle& modelHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const glm::vec3& scale
)
{
    const auto& data = staticModels_[modelHandle.id()];

	return createRenderable(renderSceneHandle, data.first, data.second, position, orientation, scale);
}

graphics::RenderableHandle GameEngine::createRenderable(
	const graphics::RenderSceneHandle& renderSceneHandle,
	const graphics::MeshHandle& meshHandle,
	const graphics::TextureHandle& textureHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const glm::vec3& scale
)
{
	return graphicsEngine_->createRenderable(renderSceneHandle, meshHandle, textureHandle, position, orientation, scale);
}

void GameEngine::animateSkeleton(
	std::vector<glm::mat4>& transformations,
    const std::chrono::duration<float32> runningTime,
	const graphics::MeshHandle& meshHandle,
	const AnimationHandle& animationHandle,
	const SkeletonHandle& skeletonHandle
)
{
    detail::checkHandleValidity(*graphicsEngine_, meshHandle);
    detail::checkHandleValidity(animations_, animationHandle);
    detail::checkHandleValidity(skeletons_, skeletonHandle);

	const auto& mesh = meshes_[meshHandle];
	const auto& animation = animations_[animationHandle];
	const auto& skeleton = skeletons_[skeletonHandle];

	transformations = std::vector<glm::mat4>(100, glm::mat4(1.0f));

	ice_engine::animateSkeleton(
		transformations,
		skeleton.globalInverseTransformation(),
		animation.animatedBoneNodes(),
		skeleton.rootBoneNode(),
		mesh.boneData(),
		animation.duration(),
		animation.ticksPerSecond(),
		runningTime
	);
}

void GameEngine::animateSkeleton(
	std::vector<glm::mat4>& transformations,
    const std::chrono::duration<float32> runningTime,
    const uint32 startFrame,
    const uint32 endFrame,
	const graphics::MeshHandle& meshHandle,
	const AnimationHandle& animationHandle,
	const SkeletonHandle& skeletonHandle
)
{
    detail::checkHandleValidity(*graphicsEngine_, meshHandle);
    detail::checkHandleValidity(animations_, animationHandle);
    detail::checkHandleValidity(skeletons_, skeletonHandle);

	const auto& mesh = meshes_[meshHandle];
	const auto& animation = animations_[animationHandle];
	const auto& skeleton = skeletons_[skeletonHandle];

	transformations = std::vector<glm::mat4>(100, glm::mat4(1.0f));

	ice_engine::animateSkeleton(
		transformations,
		skeleton.globalInverseTransformation(),
		animation.animatedBoneNodes(),
		skeleton.rootBoneNode(),
		mesh.boneData(),
		animation.duration(),
		animation.ticksPerSecond(),
		runningTime,
		startFrame,
		endFrame
	);
}

void GameEngine::handleEvents()
{
	graphicsEngine_->processEvents();

	networkingEngine_->processEvents();
}

void GameEngine::addWindowEventListener(IWindowEventListener* windowEventListener)
{
	if (std::find(windowEventListeners_.begin(), windowEventListeners_.end(), windowEventListener) != windowEventListeners_.end())
	{
		throw std::runtime_error("Window event listener already exists.");
	}

	windowEventListeners_.push_back(windowEventListener);
}

void GameEngine::addKeyboardEventListener(IKeyboardEventListener* keyboardEventListener)
{
	if (std::find(keyboardEventListeners_.begin(), keyboardEventListeners_.end(), keyboardEventListener) != keyboardEventListeners_.end())
	{
		throw std::runtime_error("Keyboard event listener already exists.");
	}

	keyboardEventListeners_.push_back(keyboardEventListener);
}

void GameEngine::addTextInputEventListener(ITextInputEventListener* textInputEventListener)
{
    if (std::find(textInputEventListeners_.begin(), textInputEventListeners_.end(), textInputEventListener) != textInputEventListeners_.end())
    {
        throw std::runtime_error("TextInput event listener already exists.");
    }

    textInputEventListeners_.push_back(textInputEventListener);
}

void GameEngine::addMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener)
{
	if (std::find(mouseMotionEventListeners_.begin(), mouseMotionEventListeners_.end(), mouseMotionEventListener) != mouseMotionEventListeners_.end())
	{
		throw std::runtime_error("Mouse motion event listener already exists.");
	}

	mouseMotionEventListeners_.push_back(mouseMotionEventListener);
}

void GameEngine::addMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener)
{
	if (std::find(mouseButtonEventListeners_.begin(), mouseButtonEventListeners_.end(), mouseButtonEventListener) != mouseButtonEventListeners_.end())
	{
		throw std::runtime_error("Mouse button event listener already exists.");
	}

	mouseButtonEventListeners_.push_back(mouseButtonEventListener);
}

void GameEngine::addMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener)
{
	if (std::find(mouseWheelEventListeners_.begin(), mouseWheelEventListeners_.end(), mouseWheelEventListener) != mouseWheelEventListeners_.end())
	{
		throw std::runtime_error("Mouse wheel event listener already exists.");
	}

	mouseWheelEventListeners_.push_back(mouseWheelEventListener);
}

void GameEngine::removeWindowEventListener(IWindowEventListener* windowEventListener)
{
    const auto it = std::find(windowEventListeners_.begin(), windowEventListeners_.end(), windowEventListener);
	if (it != windowEventListeners_.end())
	{
		windowEventListeners_.erase(it);
	}
}

void GameEngine::removeKeyboardEventListener(IKeyboardEventListener* keyboardEventListener)
{
    const auto it = std::find(keyboardEventListeners_.begin(), keyboardEventListeners_.end(), keyboardEventListener);
	if (it != keyboardEventListeners_.end())
	{
		keyboardEventListeners_.erase(it);
	}
}

void GameEngine::removeTextInputEventListener(ITextInputEventListener* textInputEventListener)
{
    const auto it = std::find(textInputEventListeners_.begin(), textInputEventListeners_.end(), textInputEventListener);
    if (it != textInputEventListeners_.end())
    {
        textInputEventListeners_.erase(it);
    }
}

void GameEngine::removeMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener)
{
    const auto it = std::find(mouseMotionEventListeners_.begin(), mouseMotionEventListeners_.end(), mouseMotionEventListener);
	if (it != mouseMotionEventListeners_.end())
	{
		mouseMotionEventListeners_.erase(it);
	}
}

void GameEngine::removeMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener)
{
    const auto it = std::find(mouseButtonEventListeners_.begin(), mouseButtonEventListeners_.end(), mouseButtonEventListener);
	if (it != mouseButtonEventListeners_.end())
	{
		mouseButtonEventListeners_.erase(it);
	}
}

void GameEngine::removeMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener)
{
    const auto it = std::find(mouseWheelEventListeners_.begin(), mouseWheelEventListeners_.end(), mouseWheelEventListener);
	if (it != mouseWheelEventListeners_.end())
	{
		mouseWheelEventListeners_.erase(it);
	}
}

void GameEngine::addWindowEventListener(void* object)
{
	scripting::ScriptObjectHandle listener(object);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "bool processEvent(const WindowEvent& in)");

	scriptWindowEventListeners_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::addKeyboardEventListener(void* object)
{
	scripting::ScriptObjectHandle listener(object);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "bool processEvent(const KeyboardEvent& in)");

	scriptKeyboardEventListeners_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::addTextInputEventListener(void* object)
{
	scripting::ScriptObjectHandle listener(object);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "bool processEvent(const TextInputEvent& in)");

	scriptTextInputEventListeners_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::addMouseMotionEventListener(void* object)
{
	scripting::ScriptObjectHandle listener(object);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "bool processEvent(const MouseMotionEvent& in)");

	scriptMouseMotionEventListeners_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::addMouseButtonEventListener(void* object)
{
	scripting::ScriptObjectHandle listener(object);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "bool processEvent(const MouseButtonEvent& in)");

	scriptMouseButtonEventListeners_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::addMouseWheelEventListener(void* object)
{
	scripting::ScriptObjectHandle listener(object);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "bool processEvent(const MouseWheelEvent& in)");

	scriptMouseWheelEventListeners_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::removeWindowEventListener(void* object)
{

}

void GameEngine::removeKeyboardEventListener(void* object)
{

}

void GameEngine::removeTextInputEventListener(void* object)
{

}

void GameEngine::removeMouseMotionEventListener(void* object)
{

}

void GameEngine::removeMouseButtonEventListener(void* object)
{

}

void GameEngine::removeMouseWheelEventListener(void* object)
{

}

void GameEngine::addConnectEventListener(IConnectEventListener* connectEventListener)
{
	if (std::find(connectEventListeners_.begin(), connectEventListeners_.end(), connectEventListener) != connectEventListeners_.end())
	{
		throw std::runtime_error("Connect event listener already exists.");
	}

	connectEventListeners_.push_back(connectEventListener);
}

void GameEngine::addDisconnectEventListener(IDisconnectEventListener* disconnectEventListener)
{
	if (std::find(disconnectEventListeners_.begin(), disconnectEventListeners_.end(), disconnectEventListener) != disconnectEventListeners_.end())
	{
		throw std::runtime_error("Mouse button event listener already exists.");
	}

	disconnectEventListeners_.push_back(disconnectEventListener);
}

void GameEngine::addMessageEventListener(IMessageEventListener* messageEventListener)
{
	if (std::find(messageEventListeners_.begin(), messageEventListeners_.end(), messageEventListener) != messageEventListeners_.end())
	{
		throw std::runtime_error("Mouse motion event listener already exists.");
	}

	messageEventListeners_.push_back(messageEventListener);
}

void GameEngine::removeConnectEventListener(IConnectEventListener* connectEventListener)
{
    const auto it = std::find(connectEventListeners_.begin(), connectEventListeners_.end(), connectEventListener);
	if (it != connectEventListeners_.end())
	{
		connectEventListeners_.erase(it);
	}
}

void GameEngine::removeDisconnectEventListener(IDisconnectEventListener* disconnectEventListener)
{
    const auto it = std::find(disconnectEventListeners_.begin(), disconnectEventListeners_.end(), disconnectEventListener);
	if (it != disconnectEventListeners_.end())
	{
		disconnectEventListeners_.erase(it);
	}
}

void GameEngine::removeMessageEventListener(IMessageEventListener* messageEventListener)
{
    const auto it = std::find(messageEventListeners_.begin(), messageEventListeners_.end(), messageEventListener);
	if (it != messageEventListeners_.end())
	{
		messageEventListeners_.erase(it);
	}
}

void GameEngine::addConnectEventListener(void* object)
{
	scripting::ScriptObjectHandle listener(object);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "bool processEvent(const ConnectEvent& in)");

	scriptConnectEventListeners_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::addDisconnectEventListener(void* object)
{
	scripting::ScriptObjectHandle listener(object);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "bool processEvent(const DisconnectEvent& in)");

	scriptDisconnectEventListeners_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::addMessageEventListener(void* object)
{
	scripting::ScriptObjectHandle listener(object);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "bool processEvent(const MessageEvent& in)");

	scriptMessageEventListeners_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::removeConnectEventListener(void* object)
{

}

void GameEngine::removeDisconnectEventListener(void* object)
{

}

void GameEngine::removeMessageEventListener(void* object)
{

}

void GameEngine::addScriptingEngineDebugHandler(IScriptingEngineDebugHandler* handler)
{
    if (std::find(scriptingEngineDebugHandlers_.begin(), scriptingEngineDebugHandlers_.end(), handler) != scriptingEngineDebugHandlers_.end())
    {
        throw std::runtime_error("Scripting engine debug handler already exists.");
    }

    scriptingEngineDebugHandlers_.push_back(handler);
}

void GameEngine::removeScriptingEngineDebugHandler(const IScriptingEngineDebugHandler* handler)
{
    const auto it = std::find(scriptingEngineDebugHandlers_.begin(), scriptingEngineDebugHandlers_.end(), handler);
    if (it != scriptingEngineDebugHandlers_.end())
    {
        scriptingEngineDebugHandlers_.erase(it);
    }
}

void GameEngine::addScriptingEngineDebugHandler(void* object)
{
    scripting::ScriptObjectHandle listener(object);
    auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(listener, "void tick(const float)");

    scriptScriptingEngineDebugHandlers_.push_back( std::make_pair(listener, scriptObjectFunctionHandle) );
}

void GameEngine::removeScriptingEngineDebugHandler(const void* object)
{

}


bool GameEngine::processEvent(const graphics::Event& event)
{
	bool consumed = false;

	switch (event.type)
	{
		case graphics::QUIT:
			std::cout << "Processing quit event." << std::endl;
			running_ = false;
			return true;
			break;

		case graphics::WINDOWEVENT:
            switch (event.window.eventType)
            {
                case graphics::WINDOWEVENT_RESIZED:
                    graphicsEngine_->setViewport(event.window.data1, event.window.data2);

                default:
                    break;
            }

			for (auto listener : windowEventListeners_)
			{
				if (listener->processEvent(event.window)) break;
			}

			for (const auto& data : scriptWindowEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event.window);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

                if (returnValue) break;
			}

			for (auto& gui : guis_)
			{
                if (gui->processEvent(event.window)) break;
			}

			break;

		case graphics::TEXTINPUT:
            for (auto listener : textInputEventListeners_)
            {
                if (listener->processEvent(event.text)) break;
            }

            for (const auto& data : scriptTextInputEventListeners_)
            {
                scripting::ParameterList arguments;
                arguments.addRef(event.text);
                uint8 returnValue = false;

                scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

                if (returnValue) break;
            }

            for (auto& gui : guis_)
            {
                if (gui->processEvent(event.text)) break;
            }

            break;

		case graphics::KEYDOWN:
		case graphics::KEYUP:
			for (auto listener : keyboardEventListeners_)
            {
                if (listener->processEvent(event.key)) break;
            }

            for (const auto& data : scriptKeyboardEventListeners_)
            {
                scripting::ParameterList arguments;
                arguments.addRef(event.key);
                uint8 returnValue = false;

                scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

                if (returnValue) break;
            }

            for (auto& gui : guis_)
            {
                if (gui->processEvent(event.key)) break;
            }

            if (event.key.keySym.scancode == graphics::SCANCODE_ESCAPE)
            {
                running_ = false;
                return true;
            }

            break;

		case graphics::MOUSEMOTION:
			for (auto listener : mouseMotionEventListeners_)
			{
                if (listener->processEvent(event.motion)) break;
			}

			for (const auto& data : scriptMouseMotionEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event.motion);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

                if (returnValue) break;
			}

            for (auto& gui : guis_)
            {
                if (gui->processEvent(event.motion)) break;
            }

			break;

		case graphics::MOUSEBUTTONDOWN:
		case graphics::MOUSEBUTTONUP:
			for (auto listener : mouseButtonEventListeners_)
			{
				if (listener->processEvent(event.button)) break;
			}

			for (const auto& data : scriptMouseButtonEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event.button);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

                if (returnValue) break;
			}

            for (auto& gui : guis_)
            {
                if (gui->processEvent(event.button)) break;
            }

			break;

		case graphics::MOUSEWHEEL:
			for (auto listener : mouseWheelEventListeners_)
			{
				if (listener->processEvent(event.wheel)) break;
			}

			for (const auto& data : scriptMouseWheelEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event.wheel);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

                if (returnValue) break;
			}

            for (auto& gui : guis_)
            {
                if (gui->processEvent(event.wheel)) break;
            }

			break;

		default:
			break;
	}

	return false;
}

bool GameEngine::processEvent(const networking::ConnectEvent& event)
{
	switch (event.type)
	{
		case networking::SERVERCONNECT:
		case networking::SERVERCONNECTFAILED:
		case networking::CLIENTCONNECT:
			for (auto listener : connectEventListeners_)
			{
				bool returnValue = listener->processEvent(event);
			}
			for (const auto& data : scriptConnectEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);
			}

			return true;
			break;

		default:
			break;
	}

	return false;
}

bool GameEngine::processEvent(const networking::DisconnectEvent& event)
{
	switch (event.type)
	{
		case networking::SERVERDISCONNECT:
		case networking::CLIENTDISCONNECT:
			for (auto listener : disconnectEventListeners_)
			{
				bool returnValue = listener->processEvent(event);
			}
			for (const auto& data : scriptDisconnectEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);
			}

			return true;
			break;

		default:
			break;
	}

	return false;
}

bool GameEngine::processEvent(const networking::MessageEvent& event)
{
	switch (event.type)
	{
		case networking::SERVERMESSAGE:
		case networking::CLIENTMESSAGE:
			for (auto listener : messageEventListeners_)
			{
				bool returnValue = listener->processEvent(event);
			}
			for (const auto& data : scriptMessageEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);
			}

			{
				std::string msg(event.message.begin(), event.message.end());
				std::cout << "Message: " << msg << std::endl;

				return true;
			}
			break;

		default:
			break;
	}

	return false;
}

void GameEngine::run()
{
	try
	{
		test();

		LOG_INFO(logger_, "We have liftoff...");

		// start our clock
		auto beginFpsTime = std::chrono::high_resolution_clock::now();
		auto endFpsTime = std::chrono::high_resolution_clock::now();
		auto previousFpsTime = beginFpsTime;
		float32 currentFps = 0.0f;
		float32 tempFps = 0.0f;
		float32 delta = 0.0f;
		float32 cumulativeDelta = 0.0f;

		//float32 runningTime;
		//std::vector<glm::mat4> transformations;

		{
//			auto filename = std::string("../data/scripts/") + bootstrapScriptName_;
//			const auto filename = bootstrapScriptName_;
//			if (!fileSystem_->exists(filename))
//			{
//				throw std::runtime_error("Script file '" + filename + "' does not exist.");
//			}

            const auto source = fileSystem_->readAll(bootstrapScriptName_, fs::FileFlags::READ);

			bootstrapModuleHandle_ = scriptingEngine_->createModule("", {source});
		}

		scriptingEngine_->execute(bootstrapModuleHandle_, "void main()");

		scriptingEngine_->execute(scriptObjectHandle_, "void initialize()");

		while ( running_ )
		{
			beginFpsTime = std::chrono::high_resolution_clock::now();
			delta = std::chrono::duration<float32>(beginFpsTime - endFpsTime).count();
			cumulativeDelta += delta;

			tempFps++;

			if (std::chrono::duration<float32>(beginFpsTime - previousFpsTime).count() > 1.0f)
			{
				previousFpsTime = beginFpsTime;
				currentFps = tempFps;
				tempFps = 0;
			}

			networkingEngine_->tick(delta);

			const float32 updateFrequency = 1.0f / 60.0f;
			int a = 0;
			while (cumulativeDelta >= updateFrequency && a < 3)
			{
				tick(updateFrequency);
				cumulativeDelta -= updateFrequency;
				a++;
			}
	//		tick(delta);

			auto beginRenderTime = std::chrono::high_resolution_clock::now();

            render();

            auto endRenderTime = std::chrono::high_resolution_clock::now();

			engineStatistics_.renderTime = std::chrono::duration<float32>(endRenderTime - beginRenderTime);

			endFpsTime = beginFpsTime;

			engineStatistics_.fps = currentFps;
		}

//		scriptingEngine_->releaseAllScriptObjects();
//		scriptingEngine_->destroyAllModules();

		destroy();
	}
	CATCH_COPY_AND_RETHROW()
}

}
