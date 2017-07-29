#include <iostream>
#include <sstream>
#include <utility>
#include <functional>
#include <algorithm>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "GameEngine.hpp"
#include "Scene.hpp"
#include "ScriptingEngineBindingDelegate.hpp"

#include "Constants.hpp"

#include "graphics/model/ModelLoader.hpp"
#include "graphics/model/Animate.hpp"

#include "graphics/GraphicsFactory.hpp"
#include "physics/PhysicsFactory.hpp"
#include "scripting/ScriptingFactory.hpp"

#include "graphics/Event.hpp"

#include "logger/Logger.hpp"
#include "fs/FileSystem.hpp"
#include "image/ImageLoader.hpp"

#include "utilities/IoUtilities.hpp"

namespace hercules
{

float32 GameEngine::rotationX = 0.0f;
float32 GameEngine::rotationY = 0.0f;
int32 GameEngine::mousePosX = 0;
int32 GameEngine::mousePosY = 0;

uint32 GameEngine::COMPONENT_TYPE_GRAPHICS = 0;

GameEngine::GameEngine(
	std::unique_ptr<utilities::Properties> properties,
	std::unique_ptr<hercules::logger::ILogger> logger
)
	: 
	properties_(std::move(properties)),
	logger_(std::move(logger))
{
	running_ = true;

	// set the initial state
	state_ = GAME_STATE_STARTUP;
	
	bootstrapScriptName_ = std::string("bootstrap.as");
	
	inConsole_ = false;
	
	initialize();
}

GameEngine::~GameEngine()
{
	logger_->info("Shutting down.");
}

GameState GameEngine::getState()
{
	return state_;
}

void GameEngine::exit()
{
	running_ = false;
}

void GameEngine::setState(GameState state)
{
	state_ = state;
}

void GameEngine::startNewGame()
{
	//DEBUG_LOG("--== Starting new game ==--")

	// Create player
	
	// create world
	//world::World::getInstance()->initialize();

	// create/load terrain

	// create ai

	// create EventManager

	// add player to the event listeners in the world

	// Create world
}

scripting::ScriptHandle scriptHandle;
scripting::ScriptObjectHandle scriptObjectHandle;
void GameEngine::tick(const float32 delta)
{
	graphicsEngine_->setMouseRelativeMode(false);
	graphicsEngine_->setCursorVisible(true);

	handleEvents();
	
	scripting::ParameterList params;
	params.add(delta);
	
	scriptingEngine_->execute(scriptObjectHandle, "void tick(const float)", params);
	
	for (auto& scene : scenes_)
	{
		scene->tick(delta);
	}
	
	/*
	// Load any opengl assets (this needs work...)
	if (openGlLoader_->getWorkQueueCount() != 0u)
	{
		openGlLoader_->tick();
	}
	*/
	
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
	
	graphicsEngine_->render(delta);
}

void GameEngine::destroy()
{
}

void GameEngine::initialize()
{
	initializeLoggingSubSystem();
	
	logger_->info( "Initializing..." );
	
	initializeFileSystemSubSystem();
	
	initializeDataStoreSubSystem();
	
	initializeThreadingSubSystem();
	
	initializeEntitySubSystem();

	initializeGraphicsSubSystem();
	
	initializePhysicsSubSystem();
	
	initializeScriptingSubSystem();

	initializeInputSubSystem();

	// Load data
	loadEssentialGameData();
	
	loadUserInterface();

	logger_->info( "Done initialization." );
}

void GameEngine::initializeLoggingSubSystem()
{
	// Initialize the log using the specified log file
	if (logger_.get() == nullptr)
	{
		logger_ = std::make_unique< logger::Logger >( std::string("hercules.log") );
	}
}

void GameEngine::initializeFileSystemSubSystem()
{
	logger_->info( "initialize file system." );
	
	fileSystem_ = std::make_unique<fs::FileSystem>();
}

void GameEngine::initializeInputSubSystem()
{
	logger_->info( "initialize keyboard and mouse." );
}
void GameEngine::initializeSoundSubSystem()
{
	logger_->info( "initialize sound." );
}

void GameEngine::initializePhysicsSubSystem()
{
	physicsEngine_ = physics::PhysicsFactory::createPhysicsEngine(properties_.get(), fileSystem_.get(), logger_.get());
}

void GameEngine::initializeGraphicsSubSystem()
{
	logger_->info( "initializing graphics." );
	
	graphicsEngine_ = graphics::GraphicsFactory::createGraphicsEngine( properties_.get(), fileSystem_.get(), logger_.get() );
	
	graphicsEngine_->addEventListener(this);
}

void GameEngine::initializeScriptingSubSystem()
{
	logger_->info( "Initializing scripting..." );
	
	scriptingEngine_ = scripting::ScriptingFactory::createScriptingEngine( properties_.get(), fileSystem_.get(), logger_.get() );
	
	ScriptingEngineBindingDelegate delegate(scriptingEngine_.get(), this, graphicsEngine_.get(), physicsEngine_.get());
	delegate.bind();
}

void GameEngine::initializeThreadingSubSystem()
{
	logger_->info( "Load thread pool..." );
	threadPool_ = std::unique_ptr<ThreadPool>( new ThreadPool() );
	
	logger_->debug( "Load opengl loader..." );
	openGlLoader_ = std::unique_ptr<OpenGlLoader>( new OpenGlLoader() );
}

void GameEngine::initializeDataStoreSubSystem()
{
	logger_->info( "Load data store..." );
	//dataStore_ = std::unique_ptr<pyliteserializer::SqliteDataStore>( new pyliteserializer::SqliteDataStore(std::string("../data/dark_horizon.db")) );
}

void GameEngine::initializeEntitySubSystem()
{
	logger_->info( "Load entity system..." );
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
	logger_->info( "load essential game data." );
}

void GameEngine::loadUserInterface()
{
	logger_->info( "load user interface." );
	
	/*
	igui_ = glrProgram_->getHtmlGui();
	
	std::string htmlUrlPath = std::string("file://") + fs::current_path().string();

	logger_->debug( "loading main.html" );
	mainGui_ = igui_->loadFromFile(htmlUrlPath + std::string("/data/ui/main.html"));
	
	auto gameGuiObject = mainGui_->createGuiObject(std::wstring(L"game"));
	
	logger_->debug( "creating main.html gui objects." );	
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
	
	logger_->debug( "Finished creating main.html gui objects." );
	
	try
	{
		mainGui_->load();
	}
	catch ( glr::exception::Exception& e)
	{
		std::stringstream msg;
		msg << "Unable to load guis: " << e.what();
		logger_->error( msg.str() );
		assert(0);
	}

	mainGui_->setVisible(true);
	*/
}

IScene* GameEngine::createScene(const std::string& name)
{
	logger_->debug( "Create Scene with name: " + name );
	
	scenes_.push_back( std::make_unique<Scene>(name, this, graphicsEngine_.get(), physicsEngine_.get(), properties_.get(), fileSystem_.get(), logger_.get(), threadPool_.get(), openGlLoader_.get()) );
	
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

void GameEngine::destroyScene(IScene* scene)
{
	auto func = [scene](const std::unique_ptr<IScene>& s) {
		return s.get() == scene;
	};
	
	auto it = std::find_if(scenes_.begin(), scenes_.end(), func);
	
	if (it != scenes_.end())
	{
		scenes_.erase(it);
	}
}

IScene* GameEngine::getScene(const std::string& name) const
{
	auto func = [&name](const std::unique_ptr<IScene>& s) {
		return s->getName() == name;
	};
	
	auto it = std::find_if(scenes_.begin(), scenes_.end(), func);
	
	if (it != scenes_.end())
	{
		return it->get();
	}
	
	return nullptr;
}

void GameEngine::setIGameInstance(asIScriptObject* obj)
{
	scriptObjectHandle = scriptingEngine_->registerScriptObject(scriptHandle, obj);
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

graphics::IGraphicsEngine* GameEngine::getGraphicsEngine() const
{
	return graphicsEngine_.get();
}

physics::IPhysicsEngine* GameEngine::getPhysicsEngine() const
{
	return physicsEngine_.get();
}

image::Image* GameEngine::loadImage(const std::string& name, const std::string& filename)
{
	logger_->debug("Loading image: " + filename);
	if (!fileSystem_->exists(filename))
	{
		throw std::runtime_error("Image file '" + filename + "' does not exist.");
	}
	
	auto file = fileSystem_->open(filename, fs::FileFlags::READ | fs::FileFlags::BINARY);
	resourceCache_.addImage( name, image::import(utilities::readAllBytes(file->getInputStream())) );
	
	return resourceCache_.getImage(name);
}

graphics::model::Model* GameEngine::loadModel(const std::string& name, const std::string& filename)
{
	throw std::logic_error("loadModel not yet implemented.");
}

graphics::model::Model* GameEngine::importModel(const std::string& name, const std::string& filename)
{
	if (!fileSystem_->exists(filename))
	{
		throw std::runtime_error("Model file '" + filename + "' does not exist.");
	}
	
	resourceCache_.addModel( name, graphics::model::import(filename, logger_.get(), fileSystem_.get()) );
	
	return resourceCache_.getModel(name);
}

void GameEngine::unloadImage(const std::string& name)
{
	return resourceCache_.removeImage(name);
}

void GameEngine::unloadModel(const std::string& name)
{
	return resourceCache_.removeModel(name);
}

image::Image* GameEngine::getImage(const std::string& name) const
{
	return resourceCache_.getImage(name);
}

graphics::model::Model* GameEngine::getModel(const std::string& name) const
{
	return resourceCache_.getModel(name);
}

ModelHandle GameEngine::loadStaticModel(const graphics::model::Model* model)
{
	auto meshHandle = graphicsEngine_->createStaticMesh(model->meshes[0].vertices, model->meshes[0].indices, model->meshes[0].colors, model->meshes[0].normals, model->meshes[0].textureCoordinates);
	auto textureHandle = graphicsEngine_->createTexture2d(model->textures[0].image);
	
	staticModels_.push_back(std::make_pair(meshHandle, textureHandle));
	
	auto index = staticModels_.size() - 1;
	
	return ModelHandle(index);
}

graphics::ShaderHandle GameEngine::createVertexShader(const std::string& name, const std::string& filename)
{
	logger_->debug("Loading vertex shader '" + name + "': " + filename);
	
	auto shaderSource = fileSystem_->readAll(filename);
	
	return createVertexShaderFromSource(name, shaderSource);
}

graphics::ShaderHandle GameEngine::createVertexShaderFromSource(const std::string& name, const std::string& data)
{
	logger_->debug("Creating vertex shader from source: " + data);
	
	if (graphicsEngine_->valid(getShader(name)))
	{
		throw std::runtime_error("Vertex shader with name '" + name + "' already exists.");
	}
	
	auto handle = graphicsEngine_->createVertexShader(data);
	
	shaderHandles_[name] = handle;
	
	return handle;
}

graphics::ShaderHandle GameEngine::createFragmentShader(const std::string& name, const std::string& filename)
{
	logger_->debug("Loading fragment shader '" + name + "': " + filename);
	
	auto shaderSource = fileSystem_->readAll(filename);
	
	return createFragmentShaderFromSource(name, shaderSource);
}

graphics::ShaderHandle GameEngine::createFragmentShaderFromSource(const std::string& name, const std::string& data)
{
	logger_->debug("Creating fragment shader from source: " + data);
	
	if (graphicsEngine_->valid(getShader(name)))
	{
		throw std::runtime_error("Fragment shader with name '" + name + "' already exists.");
	}
	
	auto handle = graphicsEngine_->createFragmentShader(data);
	
	shaderHandles_[name] = handle;
	
	return handle;
}

graphics::ShaderHandle GameEngine::getShader(const std::string& name) const
{
	auto it = shaderHandles_.find(name);
	if (it != shaderHandles_.end())
	{
		return it->second;
	}
	
	return graphics::ShaderHandle();
}

void GameEngine::destroyShader(const std::string& name)
{
	logger_->debug("Destroying shader '" + name + "'");
	
	auto it = shaderHandles_.find(name);
	if (it != shaderHandles_.end())
	{
		graphicsEngine_->destroyShader(it->second);
		shaderHandles_.erase(it);
	}
	else
	{
		logger_->warn("Cannot destroy shader '" + name + "' - shader was not found.");
	}
}

void GameEngine::destroyShader(const graphics::ShaderHandle& shaderHandle)
{
	logger_->debug("Destroying shader with id '" + std::to_string(shaderHandle.id()) + "'");
	
	auto func = [&shaderHandle](const auto& pair) {
		return pair.second == shaderHandle;
	};
	
	auto it = std::find_if(shaderHandles_.begin(), shaderHandles_.end(), func);
	
	if (it != shaderHandles_.end())
	{
		graphicsEngine_->destroyShader(it->second);
		shaderHandles_.erase(it);
	}
	else
	{
		logger_->warn("Cannot destroy shader with id '" + std::to_string(shaderHandle.id()) + "' - shader was not found.");
	}
}


graphics::ShaderProgramHandle GameEngine::createShaderProgram(const std::string& name, const graphics::ShaderHandle& vertexShaderHandle, const graphics::ShaderHandle& fragmentShaderHandle)
{
	logger_->debug("Creating shader program '" + name + "' from vertex id '" + std::to_string(vertexShaderHandle.id()) + "' and fragment id '" + std::to_string(fragmentShaderHandle.id()) + "'");
	
	if (graphicsEngine_->valid(getShaderProgram(name)))
	{
		throw std::runtime_error("Shader program with name '" + name + "' already exists.");
	}
	
	auto handle = graphicsEngine_->createShaderProgram(vertexShaderHandle, fragmentShaderHandle);
	
	shaderProgramHandles_[name] = handle;
	
	return handle;
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
	logger_->debug("Destroying shader program '" + name + "'");
	
	auto it = shaderProgramHandles_.find(name);
	if (it != shaderProgramHandles_.end())
	{
		graphicsEngine_->destroyShaderProgram(it->second);
		shaderProgramHandles_.erase(it);
	}
	else
	{
		logger_->warn("Cannot destroy shader program '" + name + "' - shader program was not found.");
	}
}

void GameEngine::destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle)
{
	logger_->debug("Destroying shader program with id '" + std::to_string(shaderProgramHandle.id()) + "'");
	
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
		logger_->warn("Cannot destroy shader program with id '" + std::to_string(shaderProgramHandle.id()) + "' - shader program was not found.");
	}
}


graphics::RenderableHandle GameEngine::createRenderable(const ModelHandle& modelHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name)
{
	auto& data = staticModels_[modelHandle.getId()];
	
	return graphicsEngine_->createRenderable(data.first, data.second, shaderProgramHandle);
}

void GameEngine::handleEvents()
{
	graphicsEngine_->processEvents();
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

void GameEngine::addKeyboardEventListener(asIScriptObject* listener)
{
	auto scriptObjectHandle = scriptingEngine_->registerScriptObject(scriptHandle, listener);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(scriptObjectHandle, "bool processEvent(const KeyboardEvent& in)");
	
	scriptKeyboardEventListeners_.push_back( std::make_pair(scriptObjectHandle, scriptObjectFunctionHandle) );
}

void GameEngine::addMouseMotionEventListener(asIScriptObject* listener)
{
	auto scriptObjectHandle = scriptingEngine_->registerScriptObject(scriptHandle, listener);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(scriptObjectHandle, "bool processEvent(const MouseMotionEvent& in)");
	
	scriptMouseMotionEventListeners_.push_back( std::make_pair(scriptObjectHandle, scriptObjectFunctionHandle) );
}

void GameEngine::addMouseButtonEventListener(asIScriptObject* listener)
{
	auto scriptObjectHandle = scriptingEngine_->registerScriptObject(scriptHandle, listener);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(scriptObjectHandle, "bool processEvent(const MouseButtonEvent& in)");
	
	scriptMouseButtonEventListeners_.push_back( std::make_pair(scriptObjectHandle, scriptObjectFunctionHandle) );
}

void GameEngine::addMouseWheelEventListener(asIScriptObject* listener)
{
	auto scriptObjectHandle = scriptingEngine_->registerScriptObject(scriptHandle, listener);
	auto scriptObjectFunctionHandle = scriptingEngine_->getScriptObjectFunction(scriptObjectHandle, "bool processEvent(const MouseWheelEvent& in)");
	
	scriptMouseWheelEventListeners_.push_back( std::make_pair(scriptObjectHandle, scriptObjectFunctionHandle) );
}

void GameEngine::removeKeyboardEventListener(asIScriptObject* listener)
{
	
}

void GameEngine::removeMouseMotionEventListener(asIScriptObject* listener)
{
	
}

void GameEngine::removeMouseButtonEventListener(asIScriptObject* listener)
{
	
}

void GameEngine::removeMouseWheelEventListener(asIScriptObject* listener)
{
	
}


bool GameEngine::processEvent(const graphics::Event& event)
{
	switch(event.type)
	{
		case graphics::QUIT:
			std::cout << "Processing quit event." << std::endl;
			running_ = false;
			return true;
			break;
		
		case graphics::KEYDOWN:
		case graphics::KEYUP:
			std::cout << "Processing key event." << std::endl;
			
			for (auto listener : keyboardEventListeners_)
			{
				bool returnValue = listener->processEvent(event.key);
			}
			for (const auto& data : scriptKeyboardEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event.key);
				uint8 returnValue = false;
				
				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);
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
				bool returnValue = listener->processEvent(event.motion);
			}
			for (const auto& data : scriptMouseMotionEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event.motion);
				uint8 returnValue = false;
				
				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);
			}
			
			break;
		
		case graphics::MOUSEBUTTONDOWN:
		case graphics::MOUSEBUTTONUP:
			for (auto listener : mouseButtonEventListeners_)
			{
				bool returnValue = listener->processEvent(event.button);
			}
			for (const auto& data : scriptMouseButtonEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event.button);
				uint8 returnValue = false;
				
				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);
			}
			break;
			
		case graphics::MOUSEWHEEL:
			for (auto listener : mouseWheelEventListeners_)
			{
				bool returnValue = listener->processEvent(event.wheel);
			}
			for (const auto& data : scriptMouseWheelEventListeners_)
			{
				scripting::ParameterList arguments;
				arguments.addRef(event.wheel);
				uint8 returnValue = false;
				
				scriptingEngine_->execute(data.first, data.second, arguments, returnValue);
			}
			break;
		
		default:
			break;
	}
	
	return false;
}

float32 GameEngine::getFps()
{
	return currentFps_;
}

uint32 GameEngine::getThreadPoolQueueSize()
{
	return threadPool_->getWorkQueueCount();
}

uint32 GameEngine::getThreadPoolWorkerCount()
{
	return threadPool_->getActiveWorkerCount() + threadPool_->getInactiveWorkerCount();
}

uint32 GameEngine::getThreadPoolActiveWorkerCount()
{
	return threadPool_->getActiveWorkerCount();
}

uint32 GameEngine::getThreadPoolInactiveWorkerCount()
{
	return threadPool_->getInactiveWorkerCount();
}

uint32 GameEngine::getOpenGlThreadPoolQueueSize()
{
	return openGlLoader_->getWorkQueueCount();
}

int32 GameEngine::getTimeForPhysics()
{
	return timeForPhysics_;
}

int32 GameEngine::getTimeForTick()
{
	return timeForTick_;
}

int32 GameEngine::getTimeForRender()
{
	return timeForRender_;
}

int32 GameEngine::getTimeForMisc()
{
	return timeForMisc_;
}

int32 GameEngine::getTimeForInput()
{
	return timeForInput_;
}

int32 GameEngine::getTimeForGuiUpdate()
{
	return timeForGuiUpdate_;
}

int32 GameEngine::getTimeForScripting()
{
	return timeForScripting_;
}

void GameEngine::run()
{
	setState(GAME_STATE_MAIN_MENU);

	int retVal = 0;

	timeForPhysics_ = 0;
	timeForTick_ = 0;
	timeForRender_ = 0;
	timeForMisc_ = 0;
	timeForInput_ = 0;
	timeForGuiUpdate_ = 0;
	timeForScripting_ = 0;
	
	test();
	
	logger_->info( "We have liftoff..." );
	
	// start our clock
	auto begin = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();
	auto previousFpsTime = begin;
	float32 tempFps = 0.0f;
	float32 delta = 0.0f;
	
	/*
	sf::Clock clock;

	sf::Time currentTime = clock.getElapsedTime();
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time previousFpsTime = clock.getElapsedTime();
	sf::Time lastGuiUpdateTime = sf::Time();
	sf::Time lastAngelscriptUpdateTime = sf::Time();
	float32 tempFps = 0.0f;
	float32 delta = 0.0f;
	*/
	setState(GAME_STATE_MAIN_MENU);
	
	int temp = 0;
	//float32 runningTime;
	//std::vector< glm::mat4 > transformations;
	
	scriptHandle = scriptingEngine_->loadScript(bootstrapScriptName_);
	scriptingEngine_->execute(scriptHandle, "void main()");
	
	scriptingEngine_->execute(scriptObjectHandle, "void initialize()");
	
	while ( running_ )
	{
		begin = std::chrono::high_resolution_clock::now();
		delta = std::chrono::duration<float32>(begin - end).count();
		
		tempFps++;
		
		if (std::chrono::duration<float32>(begin - previousFpsTime).count() > 1.0f)
		{
			previousFpsTime = begin;
			currentFps_ = tempFps;
			std::cout << "currentFps_: " << currentFps_ << " numEntities: " << scenes_[0]->getNumEntities() << std::endl;
			tempFps = 0;
		}
		
		tick(delta);
	
		end = begin;
	}
	
	scriptingEngine_->unregisterAllScriptObjects();
	scriptingEngine_->destroyAllScripts();

	//igui_->release(mainGui_);
	
	//mainGui_ = nullptr;
	
	//igui_ = nullptr;

	destroy();
}

}
