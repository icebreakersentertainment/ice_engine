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

#include "exceptions/FileNotFoundException.hpp"

#include "utilities/IoUtilities.hpp"

#define CATCH_COPY_AND_RETHROW() \
catch (const ice_engine::Exception& e) \
{ \
	throw ice_engine::Exception(e); \
} \
catch (const std::exception& e) \
{ \
	throw ice_engine::Exception(e); \
}

namespace ice_engine
{

GameEngine::GameEngine(
	std::unique_ptr<utilities::Properties> properties,
	std::unique_ptr<ice_engine::IPluginManager> pluginManager,
	std::unique_ptr<ice_engine::logger::ILogger> logger
)
	:
	properties_(std::move(properties)),
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
	std::unique_ptr<ice_engine::logger::ILogger> logger,
	std::unique_ptr<ice_engine::IPluginManager> pluginManager,
	std::unique_ptr<graphics::IGraphicsEngineFactory> graphicsEngineFactory,
	std::unique_ptr<ITerrainFactory> terrainFactory
)
	:
	properties_(std::move(properties)),
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
			logger_->error("Error while executing script function: " + std::string(e.what()));
		}

		this->releaseTemporaryExecutionContext(context);
	};

	backgroundThreadPool_->postWork(std::move(work));

	return sharedFuture;
}

void GameEngine::tick(const float32 delta)
{
	//graphicsEngine_->setMouseRelativeMode(true);
	//graphicsEngine_->setWindowGrab(false);
	//graphicsEngine_->setCursorVisible(true);

	handleEvents();

	scripting::ParameterList params;
	params.add(delta);

	scriptingEngine_->execute(scriptObjectHandle_, std::string("void tick(const float)"), params);

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

void GameEngine::destroy()
{
}

void GameEngine::initialize()
{
	running_ = true;

	bootstrapScriptName_ = std::string("bootstrap.as");

	initializeLoggingSubSystem();

	LOG_INFO(logger_, "Initializing..." );

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

	// Load data
	loadEssentialGameData();

	loadUserInterface();

	LOG_INFO(logger_, "Done initialization." );
}

void GameEngine::initializeLoggingSubSystem()
{
	// Initialize the log using the specified log file
	if (logger_.get() == nullptr)
	{
		logger_ = std::make_unique< logger::Logger >( std::string("ice_engine.log") );
	}
}

void GameEngine::initializeFileSystemSubSystem()
{
	LOG_INFO(logger_, "initialize file system." );

	fileSystem_ = std::make_unique<fs::FileSystem>();
}

void GameEngine::initializeInputSubSystem()
{
	LOG_INFO(logger_, "initialize keyboard and mouse." );
}
void GameEngine::initializeSoundSubSystem()
{
	LOG_INFO(logger_, "initialize sound." );
}

void GameEngine::initializePhysicsSubSystem()
{
	LOG_INFO(logger_, "initializing physics." );

	if (!physicsEngineFactory_)
	{
		auto physicsPlugin = pluginManager_->getPhysicsPlugin();

		if (physicsPlugin)
		{
			LOG_INFO(logger_, "initializing physics plugin " + physicsPlugin->getName() + "." );
			physicsEngineFactory_ = physicsPlugin->createFactory();

			physicsEngine_ = physicsEngineFactory_->create( properties_.get(), fileSystem_.get(), logger_.get() );

			physicsEngine_->setPhysicsDebugRenderer(debugRenderer_.get());
		}
		else
		{
			LOG_INFO(logger_, "physics system not initialized - no physics plugin found." );
//			physicsEngineFactory_ = std::make_unique<physics::PhysicsEngineFactory>();
		}
	}
}

void GameEngine::initializePathfindingSubSystem()
{
	LOG_INFO(logger_, "initializing pathfinding." );

	if (!pathfindingEngineFactory_)
	{
		auto pathfindingPlugin = pluginManager_->getPathfindingPlugin();

		if (pathfindingPlugin)
		{
			LOG_INFO(logger_, "initializing pathfinding plugin " + pathfindingPlugin->getName() + "." );
			pathfindingEngineFactory_ = pathfindingPlugin->createFactory();

			pathfindingEngine_ = pathfindingEngineFactory_->create( properties_.get(), fileSystem_.get(), logger_.get() );

			pathfindingEngine_->setPathfindingDebugRenderer(debugRenderer_.get());
		}
		else
		{
			LOG_INFO(logger_, "pathfinding system not initialized - no pathfinding plugin found." );
//			pathfindingEngineFactory_ = std::make_unique<pathfinding::PathfindingEngineFactory>();
		}
	}
}

void GameEngine::initializeGraphicsSubSystem()
{
	LOG_INFO(logger_, "initializing graphics." );

	if (!graphicsEngineFactory_)
	{
		auto graphicsPlugin = pluginManager_->getGraphicsPlugin();

		if (graphicsPlugin)
		{
			LOG_INFO(logger_, "initializing graphics plugin " + graphicsPlugin->getName() + "." );
			graphicsEngineFactory_ = graphicsPlugin->createFactory();

			graphicsEngine_ = graphicsEngineFactory_->create( properties_.get(), fileSystem_.get(), logger_.get() );

			graphicsEngine_->addEventListener(this);

			debugRenderer_ = std::make_unique<DebugRenderer>(graphicsEngine_.get());
		}
		else
		{
			LOG_INFO(logger_, "graphics system not initialized - no graphics plugin found." );
//			graphicsEngineFactory_ = std::make_unique<graphics::GraphicsEngineFactory>();
		}
	}
}

void GameEngine::initializeTerrainSubSystem()
{
	LOG_INFO(logger_, "initializing terrain." );

	if (!terrainFactory_)
	{
		terrainFactory_ = std::make_unique<TerrainFactory>();
	}

	//terrainManager_ = terrainFactory_->create( properties_.get(), fileSystem_.get(), logger_.get(), graphicsEngine_.get(), pathfindingEngine_.get(), scriptingEngine_.get(), audioEngine_.get(), networkingEngine_.get() );
}

void GameEngine::initializeModuleSubSystem()
{
	LOG_INFO(logger_, "initializing modules." );

	const auto& modulePlugins = pluginManager_->getModulePlugins();

	for (auto modulePlugin : modulePlugins)
	{
		auto module = modulePlugin->createFactory()->create(properties_.get(), fileSystem_.get(), logger_.get(), graphicsEngine_.get(), pathfindingEngine_.get(), scriptingEngine_.get(), audioEngine_.get(), networkingEngine_.get());
		modules_.push_back(std::move(module));
	}
}

void GameEngine::initializeAudioSubSystem()
{
	LOG_INFO(logger_, "initializing audio." );

	if (!audioEngineFactory_)
	{
		auto audioPlugin = pluginManager_->getAudioPlugin();

		if (audioPlugin)
		{
			LOG_INFO(logger_, "initializing audio plugin " + audioPlugin->getName() + "." );
			audioEngineFactory_ = audioPlugin->createFactory();

			audioEngine_ = audioEngineFactory_->create( properties_.get(), fileSystem_.get(), logger_.get() );
		}
		else
		{
			LOG_INFO(logger_, "audio system not initialized - no audio plugin found." );
//			audioEngineFactory_ = std::make_unique<audio::AudioEngineFactory>();
		}
	}
}

void GameEngine::initializeNetworkingSubSystem()
{
	LOG_INFO(logger_, "initializing networking." );

	if (!networkingEngineFactory_)
	{
		auto networkingPlugin = pluginManager_->getNetworkingPlugin();

		if (networkingPlugin)
		{
			LOG_INFO(logger_, "initializing networking plugin " + networkingPlugin->getName() + "." );
			networkingEngineFactory_ = networkingPlugin->createFactory();

			networkingEngine_ = networkingEngineFactory_->create( properties_.get(), fileSystem_.get(), logger_.get() );

			networkingEngine_->addEventListener(this);
		}
		else
		{
			LOG_INFO(logger_, "networking system not initialized - no networking plugin found." );
//			networkingEngineFactory_ = std::make_unique<networking::NetworkingEngineFactory>();
		}
	}
}

void GameEngine::initializeScriptingSubSystem()
{
	LOG_INFO(logger_, "Initializing scripting..." );

	scriptingEngine_ = scripting::ScriptingFactory::createScriptingEngine( properties_.get(), fileSystem_.get(), logger_.get() );

	BindingDelegate delegate(logger_.get(), scriptingEngine_.get(), this, graphicsEngine_.get(), audioEngine_.get(), networkingEngine_.get(), physicsEngine_.get(), pathfindingEngine_.get());
	delegate.bind();

	auto scriptingEngineBindingPlugins = pluginManager_->scriptingEngineBindingPlugins();

	for (auto scriptingEngineBindingPlugin : scriptingEngineBindingPlugins)
	{
		LOG_INFO(logger_, "initializing scripting engine binding plugin " + scriptingEngineBindingPlugin->getName() + "." );
		auto scriptingEngineBindingFactory = scriptingEngineBindingPlugin->createFactory();

		auto scriptingEngineBinding = scriptingEngineBindingFactory->create(properties_.get(), fileSystem_.get(), logger_.get(), graphicsEngine_.get(), pathfindingEngine_.get(), physicsEngine_.get(), scriptingEngine_.get(), audioEngine_.get(), networkingEngine_.get(), this);
		scriptingEngineBinding->bind();
		scriptingEngineBindings_.push_back(std::move(scriptingEngineBinding));
	}

	for (int i=0; i < 2; ++i)
	{
		auto context = scriptingEngine_->createExecutionContext();
		temporaryExecutionContexts_.push(context);
	}
}

void GameEngine::initializeThreadingSubSystem()
{
	LOG_INFO(logger_, "Load thread pool..." );
	backgroundThreadPool_ = std::make_unique<ThreadPool>();
	foregroundThreadPool_ = std::make_unique<ThreadPool>();

	LOG_DEBUG(logger_, "Load opengl loader..." );
	openGlLoader_ = std::make_unique<OpenGlLoader>();
	forgroundGraphicsThreadPool_ = std::make_unique<OpenGlLoader>();
}

void GameEngine::initializeDataStoreSubSystem()
{
	LOG_INFO(logger_, "Load data store..." );
	//dataStore_ = std::unique_ptr<pyliteserializer::SqliteDataStore>( new pyliteserializer::SqliteDataStore(std::string("../data/dark_horizon.db")) );
}

void GameEngine::initializeEntitySubSystem()
{
	LOG_INFO(logger_, "Load entity system..." );
}

std::vector<graphics::model::BoneData> boneData;
std::vector<graphics::model::Animation> animations;
graphics::model::BoneNode rootBoneNode;
glm::mat4 globalInverseTransformation;
graphics::SkeletonHandle skeletonHandle;
void GameEngine::test()
{

}

void GameEngine::loadEssentialGameData()
{
	LOG_INFO(logger_, "load essential game data." );
}

void GameEngine::loadUserInterface()
{
	LOG_INFO(logger_, "load user interface." );

	/*
	igui_ = glrProgram_->getHtmlGui();

	std::string htmlUrlPath = std::string("file://") + fs::current_path().string();

	LOG_DEBUG(logger_, "loading main.html" );
	mainGui_ = igui_->loadFromFile(htmlUrlPath + std::string("/data/ui/main.html"));

	auto gameGuiObject = mainGui_->createGuiObject(std::wstring(L"game"));

	LOG_DEBUG(logger_, "creating main.html gui objects." );
	{
		auto guiObject = mainGui_->createGuiObject(std::wstring(L"camera"));
		std::function<float()> function = [this]() { return camera_->getCamera()->getCamera()->getPosition().x; };
		guiObject->addFunction(std::wstring(L"getX"), function);
		function = [this]() { return camera_->getCamera()->getCamera()->getPosition().y; };
		guiObject->addFunction(std::wstring(L"getY"), function);
		function = [this]() { return camera_->getCamera()->getCamera()->getPosition().z; };
		guiObject->addFunction(std::wstring(L"getZ"), function);
	}

	{
		std::function<float()> function = [this]() { return this->getFps(); };
		gameGuiObject->addFunction(std::wstring(L"getFps"), function);
	}

	{
		std::function<uint32()> function = [this]() { return this->getThreadPoolQueueSize(); };
		gameGuiObject->addFunction(std::wstring(L"getThreadPoolQueueSize"), function);
		function = [this]() { return this->getOpenGlThreadPoolQueueSize(); };
		gameGuiObject->addFunction(std::wstring(L"getOpenGlThreadPoolQueueSize"), function);
		function = [this]() { return this->getThreadPoolWorkerCount(); };
		gameGuiObject->addFunction(std::wstring(L"getThreadPoolWorkerCount"), function);
		function = [this]() { return this->getThreadPoolActiveWorkerCount(); };
		gameGuiObject->addFunction(std::wstring(L"getThreadPoolActiveWorkerCount"), function);
		function = [this]() { return this->getThreadPoolInactiveWorkerCount(); };
		gameGuiObject->addFunction(std::wstring(L"getThreadPoolInactiveWorkerCount"), function);
	}

	{
		std::function<int32()> function = [this]() { return this->getTimeForPhysics(); };
		gameGuiObject->addFunction(std::wstring(L"getTimeForPhysics"), function);
		function = [this]() { return this->getTimeForTick(); };
		gameGuiObject->addFunction(std::wstring(L"getTimeForTick"), function);
		function = [this]() { return this->getTimeForRender(); };
		gameGuiObject->addFunction(std::wstring(L"getTimeForRender"), function);
		function = [this]() { return this->getTimeForMisc(); };
		gameGuiObject->addFunction(std::wstring(L"getTimeForMisc"), function);
		function = [this]() { return this->getTimeForInput(); };
		gameGuiObject->addFunction(std::wstring(L"getTimeForInput"), function);
		function = [this]() { return this->getTimeForGuiUpdate(); };
		gameGuiObject->addFunction(std::wstring(L"getTimeForGuiUpdate"), function);
		function = [this]() { return this->getTimeForScripting(); };
		gameGuiObject->addFunction(std::wstring(L"getTimeForScripting"), function);
	}

	{
		std::function<void()> voidFunction = [this]() { this->setState(GAME_STATE_START_NEW_GAME); };
		gameGuiObject->addFunction(std::wstring(L"startNewGame"), voidFunction);
		std::function<std::wstring()> stringFunction = []()
		{
			// Convert version string to a wide-string and return
			std::wstring wStr;
			wStr.assign(Constants::GAME_VERSION.begin(), Constants::GAME_VERSION.end());
			return wStr;
		};
		gameGuiObject->addFunction(std::wstring(L"getVersion"), stringFunction);
		std::function<uint32()> intFunction = [this]() { return static_cast<uint32>( this->getState() ); };
		gameGuiObject->addFunction(std::wstring(L"getState"), intFunction);
	}

	LOG_DEBUG(logger_, "Finished creating main.html gui objects." );

	try
	{
		mainGui_->load();
	}
	catch ( glr::exception::Exception& e)
	{
		std::stringstream msg;
		msg << "Unable to load guis: " << e.what();
		LOG_ERROR(logger_, msg.str() );
		assert(0);
	}

	mainGui_->setVisible(true);
	*/
}

Scene* GameEngine::createScene(const std::string& name, const std::vector<std::string>& scriptData, const std::string& initializationFunctionName)
{
	LOG_DEBUG(logger_, "Create Scene with name: " + name );

	auto scene = std::make_unique<Scene>(name, scriptData, initializationFunctionName, this, terrainFactory_.get(), properties_.get(), fileSystem_.get(), logger_.get());

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

	scenes_.push_back(std::move(scene));

	return scenes_.back().get();
}

void GameEngine::destroyScene(const std::string& name)
{
	auto scene = getScene(name);

	if (scene != nullptr)
	{
		destroyScene(scene);
	}
}

void GameEngine::destroyScene(Scene* scene)
{
	auto func = [scene](const std::unique_ptr<Scene>& s) {
		return s.get() == scene;
	};

	auto it = std::find_if(scenes_.begin(), scenes_.end(), func);

	if (it != scenes_.end())
	{
		scenes_.erase(it);
	}
}

Scene* GameEngine::getScene(const std::string& name) const
{
	auto func = [&name](const std::unique_ptr<Scene>& s) {
		return s->getName() == name;
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
		throw std::runtime_error("Gui plugin with name '"  + name + "' does not exist.");
	}

	auto gui = (*guiPlugin)->createFactory()->create(properties_.get(), fileSystem_.get(), logger_.get(), graphicsEngine_.get());
	guis_.push_back(std::move(gui));
	return guis_.back().get();
}

void GameEngine::destroyGui(const graphics::gui::IGui* gui)
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

void GameEngine::setCallback(graphics::gui::IMenuItem* menuItem, void* object)
{
	scripting::ScriptFunctionHandle scriptFunctionHandle(object);

	auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_.get(), scriptFunctionHandle);

	std::function<void (void)> func = [&scriptingEngine_ = scriptingEngine_, scriptFunctionHandleWrapper = scriptFunctionHandleWrapper]() {
		scriptingEngine_->execute(scriptFunctionHandleWrapper.get());
	};

	menuItem->setCallback(func);
}

Audio* GameEngine::loadAudio(const std::string& name, const std::string& filename)
{
	LOG_DEBUG(logger_, "Loading audio: " + filename);
	if (!fileSystem_->exists(filename))
	{
		throw std::runtime_error("Audio file '" + filename + "' does not exist.");
	}

	auto file = fileSystem_->open(filename, fs::FileFlags::READ | fs::FileFlags::BINARY);
	resourceCache_.addAudio( name, std::make_unique<Audio>(file->getInputStream()) );

	LOG_DEBUG(logger_, "Done loading audio: " + filename);

	return resourceCache_.getAudio(name);
}

std::shared_future<Audio*> GameEngine::loadAudioAsync(const std::string& name, const std::string& filename)
{
	auto promise = std::make_shared<std::promise<Audio*>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, promise = promise, sharedFuture = sharedFuture, name = name, filename = filename]() {
		auto audio = this->loadAudio(name, filename);
		promise->set_value(audio);
	};

	backgroundThreadPool_->postWork(std::move(func));

	return sharedFuture;
}

Image* GameEngine::loadImage(const std::string& name, const std::string& filename)
{
	LOG_DEBUG(logger_, "Loading image: " + filename);
	if (!fileSystem_->exists(filename))
	{
		throw std::runtime_error("Image file '" + filename + "' does not exist.");
	}

	auto file = fileSystem_->open(filename, fs::FileFlags::READ | fs::FileFlags::BINARY);
	resourceCache_.addImage( name, std::make_unique<Image>(file->getInputStream()) );

	LOG_DEBUG(logger_, "Done loading image: " + filename);

	return resourceCache_.getImage(name);
}

std::shared_future<Image*> GameEngine::loadImageAsync(const std::string& name, const std::string& filename)
{
	auto promise = std::make_shared<std::promise<Image*>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, promise = promise, sharedFuture = sharedFuture, name = name, filename = filename]() {
		auto image = this->loadImage(name, filename);
		promise->set_value(image);
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
		throw FileNotFoundException("Model file '" + filename + "' does not exist.");
	}

	resourceCache_.addModel( name, std::make_unique<Model>(filename, &resourceCache_, logger_.get(), fileSystem_.get()) );

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
			logger->error("Error while importing model: " + boost::diagnostic_information(e));
		}
		catch (const std::exception& e)
		{
			promise->set_exception(std::current_exception());
			logger->error("Error while importing model: " + boost::diagnostic_information(e));
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
	auto audio =  resourceCache_.getAudio(name);
	assert(audio != nullptr);

	return resourceCache_.getAudio(name);
}

Image* GameEngine::getImage(const std::string& name) const
{
	auto image =  resourceCache_.getImage(name);
	assert(image != nullptr);

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

	std::function<void()> func = [=, promise = promise, sharedFuture = sharedFuture, model = model]() {
		auto modelHandle = this->loadStaticModel(model);
		promise->set_value(modelHandle);
	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::VertexShaderHandle GameEngine::createVertexShader(const std::string& name, const std::string& filename)
{
	LOG_DEBUG(logger_, "Loading vertex shader '" + name + "': " + filename);

	auto shaderSource = fileSystem_->readAll(filename);

	return createVertexShaderFromSource(name, shaderSource);
}

std::shared_future<graphics::VertexShaderHandle> GameEngine::createVertexShaderAsync(const std::string& name, const std::string& filename)
{
	auto promise = std::make_shared<std::promise<graphics::VertexShaderHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, promise = promise, sharedFuture = sharedFuture, name = name, filename = filename]() {
		auto shaderHandle = this->createVertexShader(name, filename);
		promise->set_value(shaderHandle);
	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::VertexShaderHandle GameEngine::createVertexShaderFromSource(const std::string& name, const std::string& data)
{
	LOG_DEBUG(logger_, "Creating vertex shader from source: " + data);

	if (graphicsEngine_->valid(getVertexShader(name)))
	{
		throw std::runtime_error("Vertex shader with name '" + name + "' already exists.");
	}

	auto handle = graphicsEngine_->createVertexShader(data);

	vertexShaderHandles_[name] = handle;

	return handle;
}

std::shared_future<graphics::VertexShaderHandle> GameEngine::createVertexShaderFromSourceAsync(const std::string& name, const std::string& data)
{
	auto promise = std::make_shared<std::promise<graphics::VertexShaderHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, promise = promise, sharedFuture = sharedFuture, name = name, data = data]() {
		auto shaderHandle = this->createVertexShaderFromSource(name, data);
		promise->set_value(shaderHandle);
	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::FragmentShaderHandle GameEngine::createFragmentShader(const std::string& name, const std::string& filename)
{
	LOG_DEBUG(logger_, "Loading fragment shader '" + name + "': " + filename);

	auto shaderSource = fileSystem_->readAll(filename);

	return createFragmentShaderFromSource(name, shaderSource);
}

std::shared_future<graphics::FragmentShaderHandle> GameEngine::createFragmentShaderAsync(const std::string& name, const std::string& filename)
{
	auto promise = std::make_shared<std::promise<graphics::FragmentShaderHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, promise = promise, sharedFuture = sharedFuture, name = name, filename = filename]() {
		auto shaderHandle = this->createFragmentShader(name, filename);
		promise->set_value(shaderHandle);
	};

	openGlLoader_->postWork(std::move(func));

	return sharedFuture;
}

graphics::FragmentShaderHandle GameEngine::createFragmentShaderFromSource(const std::string& name, const std::string& data)
{
	LOG_DEBUG(logger_, "Creating fragment shader from source: " + data);

	if (graphicsEngine_->valid(getFragmentShader(name)))
	{
		throw std::runtime_error("Fragment shader with name '" + name + "' already exists.");
	}

	auto handle = graphicsEngine_->createFragmentShader(data);

	fragmentShaderHandles_[name] = handle;

	return handle;
}

std::shared_future<graphics::FragmentShaderHandle> GameEngine::createFragmentShaderFromSourceAsync(const std::string& name, const std::string& data)
{
	auto promise = std::make_shared<std::promise<graphics::FragmentShaderHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, promise = promise, sharedFuture = sharedFuture, name = name, data = data]() {
		auto shaderHandle = this->createFragmentShaderFromSource(name, data);
		promise->set_value(shaderHandle);
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
	LOG_DEBUG(logger_, "Destroying shader '" + name + "'");
	/*
	auto it = shaderHandles_.find(name);
	if (it != shaderHandles_.end())
	{
		graphicsEngine_->destroyShader(it->second);
		shaderHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy shader '" + name + "' - shader was not found.");
	}
	*/
}

void GameEngine::destroyShader(const graphics::VertexShaderHandle& shaderHandle)
{
	LOG_DEBUG(logger_, "Destroying vertex shader with id '" + std::to_string(shaderHandle.id()) + "'");

	auto func = [&shaderHandle](const auto& pair) {
		return pair.second == shaderHandle;
	};

	auto it = std::find_if(vertexShaderHandles_.begin(), vertexShaderHandles_.end(), func);

	if (it != vertexShaderHandles_.end())
	{
		graphicsEngine_->destroyShader(it->second);
		vertexShaderHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy vertex shader with id '" + std::to_string(shaderHandle.id()) + "' - shader was not found.");
	}
}

void GameEngine::destroyShader(const graphics::FragmentShaderHandle& shaderHandle)
{
	LOG_DEBUG(logger_, "Destroying fragment shader with id '" + std::to_string(shaderHandle.id()) + "'");

	auto func = [&shaderHandle](const auto& pair) {
		return pair.second == shaderHandle;
	};

	auto it = std::find_if(fragmentShaderHandles_.begin(), fragmentShaderHandles_.end(), func);

	if (it != fragmentShaderHandles_.end())
	{
		graphicsEngine_->destroyShader(it->second);
		fragmentShaderHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy fragment shader with id '" + std::to_string(shaderHandle.id()) + "' - shader was not found.");
	}
}


graphics::ShaderProgramHandle GameEngine::createShaderProgram(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle)
{
	LOG_DEBUG(logger_, "Creating shader program '" + name + "' from vertex id '" + std::to_string(vertexShaderHandle.id()) + "' and fragment id '" + std::to_string(fragmentShaderHandle.id()) + "'");

	if (graphicsEngine_->valid(getShaderProgram(name)))
	{
		throw std::runtime_error("Shader program with name '" + name + "' already exists.");
	}

	auto handle = graphicsEngine_->createShaderProgram(vertexShaderHandle, fragmentShaderHandle);

	shaderProgramHandles_[name] = handle;

	return handle;
}

std::shared_future<graphics::ShaderProgramHandle> GameEngine::createShaderProgramAsync(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle)
{
	auto promise = std::make_shared<std::promise<graphics::ShaderProgramHandle>>();
	auto sharedFuture = promise->get_future().share();

	std::function<void()> func = [=, promise = promise, sharedFuture = sharedFuture, name = name, vertexShaderHandle = vertexShaderHandle, fragmentShaderHandle = fragmentShaderHandle]() {
		auto shaderProgramHandle = this->createShaderProgram(name, vertexShaderHandle, fragmentShaderHandle);
		promise->set_value(shaderProgramHandle);
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
	LOG_DEBUG(logger_, "Destroying shader program '" + name + "'");

	auto it = shaderProgramHandles_.find(name);
	if (it != shaderProgramHandles_.end())
	{
		graphicsEngine_->destroyShaderProgram(it->second);
		shaderProgramHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy shader program '" + name + "' - shader program was not found.");
	}
}

void GameEngine::destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle)
{
	LOG_DEBUG(logger_, "Destroying shader program with id '" + std::to_string(shaderProgramHandle.id()) + "'");

	auto func = [&shaderProgramHandle](const auto& pair) {
		return pair.second == shaderProgramHandle;
	};

	auto it = std::find_if(shaderProgramHandles_.begin(), shaderProgramHandles_.end(), func);

	if (it != shaderProgramHandles_.end())
	{
		graphicsEngine_->destroyShaderProgram(it->second);
		shaderProgramHandles_.erase(it);
	}
	else
	{
		LOG_WARN(logger_, "Cannot destroy shader program with id '" + std::to_string(shaderProgramHandle.id()) + "' - shader program was not found.");
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
	auto& data = staticModels_[modelHandle.id()];

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
	std::vector< glm::mat4 >& transformations,
	float32 runningTime,
	const graphics::MeshHandle& meshHandle,
	const AnimationHandle& animationHandle,
	const SkeletonHandle& skeletonHandle
)
{
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
	std::vector< glm::mat4 >& transformations,
	float32 runningTime,
	uint32 startFrame,
	uint32 endFrame,
	const graphics::MeshHandle& meshHandle,
	const AnimationHandle& animationHandle,
	const SkeletonHandle& skeletonHandle
)
{
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
	auto it = std::find(windowEventListeners_.begin(), windowEventListeners_.end(), windowEventListener);
	if (it != windowEventListeners_.end())
	{
		windowEventListeners_.erase(it);
	}
}

void GameEngine::removeKeyboardEventListener(IKeyboardEventListener* keyboardEventListener)
{
	auto it = std::find(keyboardEventListeners_.begin(), keyboardEventListeners_.end(), keyboardEventListener);
	if (it != keyboardEventListeners_.end())
	{
		keyboardEventListeners_.erase(it);
	}
}

void GameEngine::removeMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener)
{
	auto it = std::find(mouseMotionEventListeners_.begin(), mouseMotionEventListeners_.end(), mouseMotionEventListener);
	if (it != mouseMotionEventListeners_.end())
	{
		mouseMotionEventListeners_.erase(it);
	}
}

void GameEngine::removeMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener)
{
	auto it = std::find(mouseButtonEventListeners_.begin(), mouseButtonEventListeners_.end(), mouseButtonEventListener);
	if (it != mouseButtonEventListeners_.end())
	{
		mouseButtonEventListeners_.erase(it);
	}
}

void GameEngine::removeMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener)
{
	auto it = std::find(mouseWheelEventListeners_.begin(), mouseWheelEventListeners_.end(), mouseWheelEventListener);
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
	auto it = std::find(connectEventListeners_.begin(), connectEventListeners_.end(), connectEventListener);
	if (it != connectEventListeners_.end())
	{
		connectEventListeners_.erase(it);
	}
}

void GameEngine::removeDisconnectEventListener(IDisconnectEventListener* disconnectEventListener)
{
	auto it = std::find(disconnectEventListeners_.begin(), disconnectEventListeners_.end(), disconnectEventListener);
	if (it != disconnectEventListeners_.end())
	{
		disconnectEventListeners_.erase(it);
	}
}

void GameEngine::removeMessageEventListener(IMessageEventListener* messageEventListener)
{
	auto it = std::find(messageEventListeners_.begin(), messageEventListeners_.end(), messageEventListener);
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

bool GameEngine::processEvent(const graphics::Event& event)
{
	bool consumed = false;

	switch(event.type)
	{
		case graphics::QUIT:
			std::cout << "Processing quit event." << std::endl;
			running_ = false;
			return true;
			break;

		case graphics::WINDOWEVENT:
			for (auto listener : windowEventListeners_)
			{
				if (consumed) break;
				consumed = listener->processEvent(event.window);
			}

			if (consumed) break;

			for (const auto& data : scriptWindowEventListeners_)
			{
				if (consumed) break;

				scripting::ParameterList arguments;
				arguments.addRef(event.window);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

				consumed = returnValue;
			}

			if (consumed) break;

			for (auto& gui : guis_)
			{
				if (consumed) break;

				consumed = gui->processEvent(event.window);
			}

			switch(event.window.eventType)
			{
				case graphics::WINDOWEVENT_RESIZED:
					graphicsEngine_->setViewport(event.window.data1, event.window.data2);

				default:
					break;
			}

			break;

		case graphics::KEYDOWN:
		case graphics::KEYUP:
			for (auto& gui : guis_)
			{
				if (consumed) break;
				consumed = gui->processEvent(event.key);
			}

			if (consumed) break;

			for (auto listener : keyboardEventListeners_)
			{
				if (consumed) break;
				consumed = listener->processEvent(event.key);
			}

			if (consumed) break;

			for (const auto& data : scriptKeyboardEventListeners_)
			{
				if (consumed) break;

				scripting::ParameterList arguments;
				arguments.addRef(event.key);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

				consumed = returnValue;
			}

			if (consumed) break;

			if (event.key.keySym.scancode == graphics::SCANCODE_ESCAPE)
			{
				running_ = false;
				return true;
			}

			break;

		case graphics::MOUSEMOTION:
			for (auto& gui : guis_)
			{
				if (consumed) break;
				consumed = gui->processEvent(event.motion);
			}

			for (auto listener : mouseMotionEventListeners_)
			{
				if (consumed) break;
				consumed = listener->processEvent(event.motion);
			}

			for (const auto& data : scriptMouseMotionEventListeners_)
			{
				if (consumed) break;

				scripting::ParameterList arguments;
				arguments.addRef(event.motion);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

				consumed = returnValue;
			}

			if (consumed) break;

			break;

		case graphics::MOUSEBUTTONDOWN:
		case graphics::MOUSEBUTTONUP:
			for (auto& gui : guis_)
			{
				if (consumed) break;
				consumed = gui->processEvent(event.button);
			}

			if (consumed) break;

			for (auto listener : mouseButtonEventListeners_)
			{
				if (consumed) break;
				consumed = listener->processEvent(event.button);
			}

			if (consumed) break;

			for (const auto& data : scriptMouseButtonEventListeners_)
			{
				if (consumed) break;

				scripting::ParameterList arguments;
				arguments.addRef(event.button);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

				consumed = returnValue;
			}

			break;

		case graphics::MOUSEWHEEL:
			for (auto& gui : guis_)
			{
				if (consumed) break;
				consumed = gui->processEvent(event.wheel);
			}

			if (consumed) break;

			for (auto listener : mouseWheelEventListeners_)
			{
				if (consumed) break;
				consumed = listener->processEvent(event.wheel);
			}

			if (consumed) break;

			for (const auto& data : scriptMouseWheelEventListeners_)
			{
				if (consumed) break;

				scripting::ParameterList arguments;
				arguments.addRef(event.wheel);
				uint8 returnValue = false;

				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);

				consumed = returnValue;
			}

			break;

		default:
			break;
	}

	return false;
}

bool GameEngine::processEvent(const networking::ConnectEvent& event)
{
	switch(event.type)
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
	switch(event.type)
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
	switch(event.type)
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

		LOG_INFO( logger_, "We have liftoff..." );

		// start our clock
		auto beginFpsTime = std::chrono::high_resolution_clock::now();
		auto endFpsTime = std::chrono::high_resolution_clock::now();
		auto previousFpsTime = beginFpsTime;
		float32 currentFps = 0.0f;
		float32 tempFps = 0.0f;
		float32 delta = 0.0f;
		float32 cumulativeDelta = 0.0f;

		//float32 runningTime;
		//std::vector< glm::mat4 > transformations;

		{
			auto filename = std::string("../data/scripts/") + bootstrapScriptName_;
			if (!fileSystem_->exists(filename))
			{
				throw std::runtime_error("Script file '" + filename + "' does not exist.");
			}

			auto source = fileSystem_->readAll(filename, fs::FileFlags::READ);

			bootstrapModuleHandle_ = scriptingEngine_->createModule("", {source});
		}

		scriptingEngine_->execute(bootstrapModuleHandle_, std::string("void main()"));

		scriptingEngine_->execute(scriptObjectHandle_, std::string("void initialize()"));

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

			// test animation
			/*
			transformations = std::vector< glm::mat4 >(100, glm::mat4(1.0));
			auto animatedBoneNodes = animations[0].animatedBoneNodes;
			auto duration = animations[0].duration;
			auto ticksPerSecond = animations[0].ticksPerSecond;
			runningTime += 0.01;
			graphics::model::animateSkeleton(transformations, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData[0], duration, ticksPerSecond, runningTime, 0, 0);
			graphicsEngine_->update(skeletonHandle, &transformations[0], 100 * sizeof(glm::mat4));
			*/

			//transformations = std::vector< glm::mat4 >(100, glm::mat4(1.0));
			//graphicsEngine_->update(skeletonHandle, &transformations[0], 100 * sizeof(glm::mat4));

			auto beginRenderTime = std::chrono::high_resolution_clock::now();

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

			auto endRenderTime = std::chrono::high_resolution_clock::now();

			engineStatistics_.renderTime = std::chrono::duration<float32>(endRenderTime - beginRenderTime).count();

			endFpsTime = beginFpsTime;

			engineStatistics_.fps = currentFps;
		}

		scriptingEngine_->releaseAllScriptObjects();
		scriptingEngine_->destroyAllModules();

		destroy();
	}
	CATCH_COPY_AND_RETHROW()
}

}
