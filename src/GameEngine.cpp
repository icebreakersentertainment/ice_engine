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

#include "Constants.hpp"

#include "graphics/model/ModelLoader.hpp"
#include "graphics/model/Animate.hpp"

#include "graphics/GraphicsFactory.hpp"
#include "scripting/ScriptingFactory.hpp"

#include "graphics/Event.hpp"

#include "logger/Logger.hpp"
#include "fs/FileSystem.hpp"
#include "utilities/ImageLoader.hpp"

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
	player_ = std::unique_ptr<Player>( new Player(/*camera_.get()*/) );
	// create world
	//world::World::getInstance()->initialize();

	// create/load terrain

	// create ai

	// create EventManager

	// add player to the event listeners in the world

	// Create world
}

void GameEngine::tick(float32 elapsedTime)
{
	switch ( state_ )
	{
		case GAME_STATE_UNKNOWN:
			break;
		
		case GAME_STATE_MAIN_MENU:
			
			break;
	
	
		case GAME_STATE_IN_GAME:
			
			// TODO: Probably need to rethink how I do this?
			//smgr_->getTerrainManager( nullptr )->update(1);
			// Load any opengl assets (this needs work...)
			openGlLoader_->tick();
			openGlLoader_->tick();
			openGlLoader_->tick();
			openGlLoader_->tick();
			openGlLoader_->tick();
			break;
	
		default:
			break;
	}
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
	//sfmlWindow_ = (sf::Window*)window_->getInternalWindowPointer();
	//sfmlWindow_->setKeyRepeatEnabled(false);
}
void GameEngine::initializeSoundSubSystem()
{
	logger_->info( "initialize sound." );
}

void GameEngine::initializePhysicsSubSystem()
{
}

void GameEngine::initializeGraphicsSubSystem()
{
	logger_->info( "initializing graphics." );
	
	//properties_->getIntValue( std::string("window.depth") );
	//properties_->getBoolValue( std::string("window.fullscreen") );
	//properties_->getBoolValue( std::string("window.vsync") );
	
	graphicsEngine_ = graphics::GraphicsFactory::createGraphicsEngine( properties_.get(), fileSystem_.get(), logger_.get() );
	
	graphicsEngine_->addEventListener(this);
	
	/*
	logger_->debug( "create glr program." );
	glr::ProgramSettings settings = glr::ProgramSettings();
	settings.defaultTextureDir = std::string( Constants::MODELS_DIRECTORY );
	glrProgram_ = std::unique_ptr<glr::GlrProgram>( new glr::GlrProgram(settings) );

	logger_->debug( "create window." );
	window_ = glrProgram_->createWindow(
		Constants::GAME_NAME,
		Constants::GAME_NAME,
		properties_->getIntValue( std::string("window.width") ),
		properties_->getIntValue( std::string("window.height") ),
		properties_->getIntValue( std::string("window.depth") ),
		properties_->getBoolValue( std::string("window.fullscreen") ),
		properties_->getBoolValue( std::string("window.vsync") )
	);
	
	assert(glrProgram_->getOpenGlDevice() != nullptr);
	assert(glrProgram_->getOpenGlDevice()->getShaderProgramManager() != nullptr);
	
	logger_->debug( "create scene manager." );

	smgr_ = glrProgram_->getSceneManager();
	
	assert(smgr_ != nullptr);
	
	logger_->debug( "Initializing custom shaders..." );
	smgr_->getShaderProgramManager()->loadShaderPrograms(std::string("./data/shaders"));
	
	logger_->debug( "create camera." );
	glr::ICamera* camera = smgr_->createCamera();
	camera->setPosition(13.0f, 8.0f, -6.0f);
	camera->lookAt( glm::vec3(24.0f, 3.0f, 24.0f) );
	
	auto fpsCamera = std::unique_ptr< glr::extras::FpsCamera >( new glr::extras::FpsCamera(camera, 0.060f) );
	camera_ = std::unique_ptr< Camera >( new Camera( std::move(fpsCamera) ) );
	*/
}

void GameEngine::initializeScriptingSubSystem()
{
	logger_->info( "Initializing scripting..." );
	
	scriptingEngine_ = scripting::ScriptingFactory::createScriptingEngine( properties_.get(), fileSystem_.get(), logger_.get() );
	
	// Types available in the scripting engine
	scriptingEngine_->registerObjectType("Entity", sizeof(entities::Entity), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::Entity>());
	scriptingEngine_->registerClassMethod("Entity", "uint64 getId() const", asMETHODPR(entities::Entity, getId, () const, uint64));
	
	scriptingEngine_->registerObjectType("ModelHandle", sizeof(ModelHandle), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<ModelHandle>());
	scriptingEngine_->registerClassMethod("ModelHandle", "int32 getId() const", asMETHODPR(ModelHandle, getId, () const, int32));
	scriptingEngine_->registerObjectType("RenderableHandle", sizeof(graphics::RenderableHandle), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<graphics::RenderableHandle>());
	scriptingEngine_->registerClassMethod("RenderableHandle", "int32 getId() const", asMETHODPR(graphics::RenderableHandle, getId, () const, int32));
	scriptingEngine_->registerObjectType("CollisionShapeHandle", sizeof(physics::CollisionShapeHandle), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<physics::CollisionShapeHandle>());
	scriptingEngine_->registerClassMethod("CollisionShapeHandle", "int32 getId() const", asMETHODPR(physics::CollisionShapeHandle, getId, () const, int32));
	scriptingEngine_->registerObjectType("CollisionBodyHandle", sizeof(physics::CollisionBodyHandle), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<physics::CollisionBodyHandle>());
	scriptingEngine_->registerClassMethod("CollisionBodyHandle", "int32 getId() const", asMETHODPR(physics::CollisionBodyHandle, getId, () const, int32));
	
	scriptingEngine_->registerObjectType("GraphicsComponent", sizeof(entities::GraphicsComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::GraphicsComponent>());
	scriptingEngine_->registerObjectProperty("GraphicsComponent", "vec3 scale", asOFFSET(entities::GraphicsComponent, scale));
	scriptingEngine_->registerObjectProperty("GraphicsComponent", "RenderableHandle renderableHandle", asOFFSET(entities::GraphicsComponent, renderableHandle));
	scriptingEngine_->registerObjectType("PhysicsComponent", sizeof(entities::PhysicsComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::PhysicsComponent>());
	scriptingEngine_->registerObjectProperty("PhysicsComponent", "CollisionBodyHandle collisionBodyHandle", asOFFSET(entities::PhysicsComponent, collisionBodyHandle));
	scriptingEngine_->registerObjectType("PositionOrientationComponent", sizeof(entities::PositionOrientationComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::PositionOrientationComponent>());
	scriptingEngine_->registerObjectProperty("PositionOrientationComponent", "vec3 position", asOFFSET(entities::PositionOrientationComponent, position));
	scriptingEngine_->registerObjectProperty("PositionOrientationComponent", "quat orientation", asOFFSET(entities::PositionOrientationComponent, orientation));
	
	// Register Model/Mesh/etc
	//RegisterVectorBindings<glm::vec3>(engine_, "vectorVec3", "vec3");
	//scriptingEngine_->registerObjectType("Mesh", 0, asOBJ_REF | asOBJ_NOCOUNT);
	//scriptingEngine_->registerObjectProperty("Mesh", "vectorMVec3 vertices", asOFFSET(graphics::model::Mesh, vertices));
	//scriptingEngine_->registerObjectType("Model", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectType("Model", sizeof(graphics::model::Model), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<graphics::model::Model>());
	//scriptingEngine_->registerObjectProperty("Model", "vectorMesh meshes", asOFFSET(graphics::model::Model, meshes));
	
	// IScene
	scriptingEngine_->registerObjectType("IScene", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod("IScene", "string getName() const", asMETHODPR(IScene, getName, () const, std::string));
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionShapeHandle createStaticPlaneShape(const vec3& in, const float)",
		asMETHODPR(IScene, createStaticPlaneShape, (const glm::vec3&, const float32), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionShapeHandle createStaticBoxShape(const vec3& in)",
		asMETHODPR(IScene, createStaticBoxShape, (const glm::vec3&), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void destroyStaticShape(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, destroyStaticShape, (const physics::CollisionShapeHandle&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void destroyAllStaticShapes()",
		asMETHODPR(IScene, destroyAllStaticShapes, (), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionBodyHandle createDynamicRigidBody(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, createDynamicRigidBody, (const physics::CollisionShapeHandle&), physics::CollisionBodyHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionBodyHandle createDynamicRigidBody(const CollisionShapeHandle& in, const float, const float, const float)",
		asMETHODPR(IScene, createDynamicRigidBody, (const physics::CollisionShapeHandle&, const float32, const float32, const float32), physics::CollisionBodyHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionBodyHandle createDynamicRigidBody(const CollisionShapeHandle& in, const vec3& in, const quat& in, const float, const float, const float)",
		asMETHODPR(IScene, createDynamicRigidBody, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&, const float32, const float32, const float32), physics::CollisionBodyHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionBodyHandle createStaticRigidBody(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, createStaticRigidBody, (const physics::CollisionShapeHandle&), physics::CollisionBodyHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionBodyHandle createStaticRigidBody(const CollisionShapeHandle& in, const float, const float)",
		asMETHODPR(IScene, createStaticRigidBody, (const physics::CollisionShapeHandle&, const float32, const float32), physics::CollisionBodyHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionBodyHandle createStaticRigidBody(const CollisionShapeHandle& in, const vec3& in, const quat& in, const float, const float)",
		asMETHODPR(IScene, createStaticRigidBody, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&, const float32, const float32), physics::CollisionBodyHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RenderableHandle createRenderable(const ModelHandle& in, const string& in = string())",
		asMETHODPR(IScene, createRenderable, (const ModelHandle&, const std::string&), graphics::RenderableHandle)
	);
	scriptingEngine_->registerClassMethod("IScene", "Entity createEntity()", asMETHODPR(IScene, createEntity, (), entities::Entity));
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void assign(const Entity& in, const GraphicsComponent& in)", 
		asMETHODPR(IScene, assign, (const entities::Entity&, const entities::GraphicsComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void assign(const Entity& in, const PhysicsComponent& in)", 
		asMETHODPR(IScene, assign, (const entities::Entity&, const entities::PhysicsComponent&), void)
	);
	/*
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void rotate(const Entity& in, const quat& in)",
		asMETHODPR(IScene, rotate, (const entities::Entity&, const glm::quat&), void)
	);
	*/
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void rotation(const Entity& in, const quat& in)",
		asMETHODPR(IScene, rotation, (const entities::Entity&, const glm::quat&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void scale(const Entity& in, const float)",
		asMETHODPR(IScene, scale, (const entities::Entity&, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void scale(const Entity& in, const vec3& in)",
		asMETHODPR(IScene, scale, (const entities::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void scale(const Entity& in, const float, const float, const float)",
		asMETHODPR(IScene, scale, (const entities::Entity&, const float32, const float32, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void translate(const Entity& in, const vec3& in)",
		asMETHODPR(IScene, translate, (const entities::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void position(const Entity& in, const vec3& in)",
		asMETHODPR(IScene, position, (const entities::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void position(const Entity& in, const float, const float, const float)",
		asMETHODPR(IScene, position, (const entities::Entity&, const float32, const float32, const float32), void)
	);
	
	// IGameEngine functions available in the scripting engine
	scriptingEngine_->registerGlobalFunction(
		"Model importModel(const string& in)",
		asMETHODPR(IGameEngine, importModel, (const std::string&) const, graphics::model::Model),
		asCALL_THISCALL_ASGLOBAL,
		this
	);
	
	scriptingEngine_->registerGlobalFunction(
		"ModelHandle loadStaticModel(const Model& in)",
		asMETHODPR(IGameEngine, loadStaticModel, (const graphics::model::Model&), ModelHandle),
		asCALL_THISCALL_ASGLOBAL,
		this
	);
	
	scriptingEngine_->registerGlobalFunction(
		"IScene@ createScene(const string& in)",
		asMETHODPR(IGameEngine, createScene, (const std::string&), IScene*),
		asCALL_THISCALL_ASGLOBAL,
		this
	);
	
	scriptingEngine_->registerGlobalFunction(
		"IScene@ getScene(const string& in)",
		asMETHODPR(IGameEngine, getScene, (const std::string&) const, IScene*),
		asCALL_THISCALL_ASGLOBAL,
		this
	);
	
	// TESTING - loading scripts and creating objects
	//scriptingEngine_->loadScript(std::string("Main"), std::string("Main.as"));
	
	//auto asObject = scriptingEngine_->createAsObject(std::string("Main"), std::string("Main"));
	
	//asObject->callMethod( std::string("void tick()") );
	
	/*
	// Register Classes available to scripts
	scriptingEngine_->registerClass(
		std::string("Entity"),
		std::string("Entity@ f()"),
		std::string("void f()"),
		std::string("void f()"),
		asFUNCTION(entities::EntityFactory::entityRefFactory),
		asMETHOD(entities::Entity, addRef),
		asMETHOD(entities::Entity, releaseRef)
	);
	
	scriptingEngine_->registerClass(
		std::string("GraphicsComponent"),
		std::string("GraphicsComponent@ f()"),
		std::string("void f()"),
		std::string("void f()"),
		asFUNCTION(entities::GraphicsComponentFactory::graphicsComponentRefFactory),
		asMETHOD(entities::GraphicsComponent, addRef),
		asMETHOD(entities::GraphicsComponent, releaseRef)
	);
	
	scriptingEngine_->registerClassMethod(std::string("Entity"), std::string("void addComponent(GraphicsComponent@)"), asMETHODPR(entities::Entity, addComponent, (entities::GraphicsComponent*), void));
	
	scriptingEngine_->registerClassMethod(std::string("GraphicsComponent"), std::string("void rotate(float, const vec3 &in)"), asMETHODPR(entities::GraphicsComponent, rotate, (float, const Vec3 &), void));
	scriptingEngine_->registerClassMethod(std::string("GraphicsComponent"), std::string("void translate(const vec3 &in)"), asMETHODPR(entities::GraphicsComponent, translate, (const Vec3 &), void));
	scriptingEngine_->registerClassMethod(std::string("GraphicsComponent"), std::string("void setScale(float, float, float)"), asMETHODPR(entities::GraphicsComponent, setScale, (float, float, float), void));
	scriptingEngine_->registerClassMethod(std::string("GraphicsComponent"), std::string("void lookAt(const vec3 &in)"), asMETHODPR(entities::GraphicsComponent, lookAt, (const Vec3 &), void));
	scriptingEngine_->registerClassMethod(std::string("GraphicsComponent"), std::string("void setPosition(const vec3 &in)"), asMETHODPR(entities::GraphicsComponent, setPosition, (const Vec3 &), void));
	scriptingEngine_->registerClassMethod(std::string("GraphicsComponent"), std::string("void setPosition(float, float, float)"), asMETHODPR(entities::GraphicsComponent, setPosition, (float, float, float), void));
	
	// Global functions
	scriptingEngine_->registerGlobalFunction(
		std::string("Entity@ getEntity(const string& in)"),
		asMETHODPR(IGameEngine, getEntity, (const std::string&), entities::Entity*),
		asCALL_THISCALL_ASGLOBAL,
		this
	);
	
	scriptingEngine_->registerGlobalFunction(
		std::string("Entity@ createEntity()"),
		asMETHODPR(IGameEngine, createEntity, (), entities::Entity*),
		asCALL_THISCALL_ASGLOBAL,
		this
	);
	
	// Set global constants
	scriptingEngine_->registerGlobalProperty(std::string("const uint COMPONENT_TYPE_GRAPHICS"), &GameEngine::COMPONENT_TYPE_GRAPHICS);
	
	// TESTING - loading scripts and creating objects
	scriptingEngine_->loadScript(std::string("Main"), std::string("Main.as"));
	
	auto asObject = scriptingEngine_->createAsObject(std::string("Main"), std::string("Main"));
	
	asObject->callMethod( std::string("void tick()") );
	*/
	/*
	scriptingEngine_ = std::unique_ptr<as_wrapper::Scripting>( new as_wrapper::Scripting() );
	
	bool success = true;
	success = success && scriptingEngine_->registerClass(
		std::string("TestAtom"), 
		std::string("TestAtom@ f()"), 
		std::string("void f()"), 
		std::string("void f()"), 
		asFUNCTION(angel_script::ClassFactory::atomRefFactory), 
		asMETHOD(angel_script::TestAtom, addRef), 
		asMETHOD(angel_script::TestAtom, releaseRef)
	);
	
	success = success && scriptingEngine_->registerClassMethod(std::string("TestAtom"), std::string("void test()"), asMETHOD(angel_script::TestAtom, test));
	
	
	
	if ( !success )
	{
		logger_->warn( "Warning: Not all object/method registration completed successfully!" );
	}

	// load all of the scripts
	scriptingEngine_->loadScripts();

	// testing
	scriptingEngine_->initContext( std::string("void eat(TestAtom atom)"), std::string("test"));
	angel_script::TestAtom* atom = new angel_script::TestAtom();
	scriptingEngine_->setArgObject(0, atom);
	delete atom;

	//scriptingEngine_->initContext(text, "test");

	int r = scriptingEngine_->run();
	scriptingEngine_->releaseContext();
	*/
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
	//logger_->info( "Load entity system..." );
	//entityEvents_ = entityx::ptr<entityx::EventManager>(new entityx::EventManager());
	//entities_ = entityx::ptr<entityx::EntityManager>(new entityx::EntityManager(entityEvents_));
}

std::vector<graphics::model::BoneData> boneData;
std::vector<graphics::model::Animation> animations;
graphics::model::BoneNode rootBoneNode;
glm::mat4 globalInverseTransformation;
graphics::SkeletonHandle skeletonHandle;
void GameEngine::test()
{
	cameraHandle_ = graphicsEngine_->createCamera(glm::vec3(0.0f, 0.0f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f));
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

/*
void GameEngine::receiveKeyboardEvent(sf::Event evt)
{
	int wvmods = 0;
	
	switch(evt.type)
	{
	case sf::Event::TextEntered:
		if ( evt.text.unicode == '`' || evt.text.unicode == '~' )
		{
			inConsole_ = !inConsole_;
		}
		else
		{
			//igui_->keyEvent(true, wvmods, evt.text.unicode, 0);
			//igui_->keyEvent(false, wvmods, evt.text.unicode, 0);
		}

		std::cout << "textentered: " << evt.text.unicode << std::endl;
	
	case sf::Event::KeyPressed:
		{
			switch(evt.key.code)
			{
			case sf::Keyboard::Left:
				//igui_->keyEvent(true, wvmods, 276, 0);
				break;
			case sf::Keyboard::Up:
				//igui_->keyEvent(true, wvmods, 273, 0);
				break;
			case sf::Keyboard::Right:
				//igui_->keyEvent(true, wvmods, 275, 0);
				break;
			case sf::Keyboard::Down:
				//igui_->keyEvent(true, wvmods, 274, 0);
				break;
			default:
				break;
			}
		}
	
	case sf::Event::KeyReleased:
		{
			switch(evt.key.code)
			{
			case sf::Keyboard::Left:
				//igui_->keyEvent(false, wvmods, 276, 0);
				break;
			case sf::Keyboard::Up:
				//igui_->keyEvent(false, wvmods, 273, 0);
				break;
			case sf::Keyboard::Right:
				//igui_->keyEvent(false, wvmods, 275, 0);
				break;
			case sf::Keyboard::Down:
				//igui_->keyEvent(false, wvmods, 274, 0);
				break;
			default:
				break;
			}
		}

	default:
		break;
	}
	
	// reload shaders
	if (evt.text.unicode == 44) // /u44 = comma key
	{
		//glrProgram_->reloadShaders();
	}
}
*/

/*
void GameEngine::receiveMouseEvent(sf::Event evt)
{
	switch (evt.type)
	{
	case sf::Event::MouseMoved:
		//GameEngine::rotationX = (-1.0f) * (float32)(evt.mouseMove.x - GameEngine::mousePosX);
		//GameEngine::rotationY = (-1.0f) * (float32)(evt.mouseMove.y - GameEngine::mousePosY);
		//GameEngine::mousePosX = evt.mouseMove.x;
		//GameEngine::mousePosY = evt.mouseMove.y;

		//igui_->mouseMoved(evt.mouseMove.x, evt.mouseMove.y);
		break;

	case sf::Event::MouseButtonPressed:
		switch(evt.mouseButton.button)
		{
		case sf::Mouse::Left:
			//igui_->mouseButton(0, evt.mouseButton.x, evt.mouseButton.y, true);
			break;
		
		case sf::Mouse::Right:
			//igui_->mouseButton(1, evt.mouseButton.x, evt.mouseButton.y, true);
			break;
		
		default:
			break;
		}
		
		break;

	case sf::Event::MouseButtonReleased:
		switch(evt.mouseButton.button)
		{
		case sf::Mouse::Left:
			//igui_->mouseButton(0, evt.mouseButton.x, evt.mouseButton.y, false);
			break;
		
		case sf::Mouse::Right:
			//igui_->mouseButton(1, evt.mouseButton.x, evt.mouseButton.y, false);
			break;
		
		default:
			break;
		}

		break;

	case sf::Event::MouseWheelMoved:
	{
		// TODO: Make a constant for this
		float32 mouseScrollWheelSensitivity = 15.0f;
		//igui_->mouseWheel(evt.mouseWheel.x, evt.mouseWheel.y, 0, (int32)(mouseScrollWheelSensitivity * evt.mouseWheel.delta));
	}
	default:
		break;
	}
	
	
}
*/

IScene* GameEngine::createScene(const std::string& name)
{
	logger_->debug( "Create Scene with name: " + name );
	
	scenes_.push_back( std::make_unique<Scene>(name, this, graphicsEngine_.get(), properties_.get(), fileSystem_.get(), logger_.get(), threadPool_.get(), openGlLoader_.get()) );
	
	return scenes_.back().get();
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

graphics::model::Model GameEngine::importModel(const std::string& filename) const
{
	if (!fileSystem_->exists(filename))
	{
		throw std::runtime_error("Model file '" + filename + "' does not exist.");
	}
	
	auto model = graphics::model::import(filename, logger_.get(), fileSystem_.get());
	
	return std::move(model);
}

ModelHandle GameEngine::loadStaticModel(const graphics::model::Model& model)
{
	auto meshHandle = graphicsEngine_->createStaticMesh(model.meshes[0].vertices, model.meshes[0].indices, model.meshes[0].colors, model.meshes[0].normals, model.meshes[0].textureCoordinates);
	auto textureHandle = graphicsEngine_->createTexture2d(model.textures[0].image);
	
	staticModels_.push_back(std::tuple<graphics::MeshHandle, graphics::TextureHandle>(meshHandle, textureHandle));
	
	auto index = staticModels_.size() - 1;
	
	return ModelHandle(index);
}

graphics::RenderableHandle GameEngine::createRenderable(const ModelHandle& modelHandle, const std::string& name)
{
	auto data = staticModels_[modelHandle.getId()];
	
	return graphicsEngine_->createRenderable(std::get<0>(data), std::get<1>(data));
}

void GameEngine::handleEvents()
{
	graphicsEngine_->processEvents();
	/*
	sf::Event event;

	// while there are pending events...
	while ( sfmlWindow_->pollEvent(event) )
	{
		switch ( event.type )
		{
		// window closed
		case sf::Event::Closed:
			//window.close();
			break;

		// key pressed
		//case sf::Event::KeyPressed:
		//	break;

		case sf::Event::Resized:
			window_->resize(event.size.width, event.size.height);
			break;

		// process keyboard and mouse events
		default:
			receiveKeyboardEvent(event);
			receiveMouseEvent(event);
			break;
		}
	}
	*/
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
			std::cout << "Processing key down event." << std::endl;
			
			if (event.key.keySym.scancode == graphics::SCANCODE_ESCAPE)
			{
				running_ = false;
				return true;
			}
			
			break;
			
		case graphics::MOUSEMOTION:
			{
				const auto degrees = event.motion.xrel / 5.5f;
				graphicsEngine_->rotate(cameraHandle_, -degrees, glm::vec3(0.0f, 1.0f, 0.0f), graphics::TransformSpace::TS_LOCAL);
			}
			{
				const auto degrees = event.motion.yrel / 5.5f;
				graphicsEngine_->rotate(cameraHandle_, -degrees, glm::vec3(1.0f, 0.0f, 0.0f), graphics::TransformSpace::TS_WORLD);
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

float32 runningTime;
std::vector< glm::mat4 > transformations;
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
	float32 deltaTime = 0.0f;
	
	/*
	sf::Clock clock;

	sf::Time currentTime = clock.getElapsedTime();
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time previousFpsTime = clock.getElapsedTime();
	sf::Time lastGuiUpdateTime = sf::Time();
	sf::Time lastAngelscriptUpdateTime = sf::Time();
	float32 tempFps = 0.0f;
	float32 deltaTime = 0.0f;
	*/
	setState(GAME_STATE_MAIN_MENU);
	
	int temp = 0;
	
	scriptingEngine_->runScript(bootstrapScriptName_);
	
	while ( running_ )
	{
		begin = std::chrono::high_resolution_clock::now();
		deltaTime = std::chrono::duration<float32>(begin - end).count();
		
		//std::cout << "deltaTime: " << deltaTime << std::endl;
		
		switch ( state_ )
		{
			case GAME_STATE_MAIN_MENU:
				//sfmlWindow_->setMouseCursorVisible(true);
				graphicsEngine_->setMouseRelativeMode(false);
				graphicsEngine_->setCursorVisible(true);
				//camera_->tick(deltaTime);
	
				handleEvents();
				
				tempFps++;
				
				if (std::chrono::duration<float32>(begin - previousFpsTime).count() > 1.0f)
				{
					previousFpsTime = begin;
					currentFps_ = tempFps;
					std::cout << "currentFps_: " << currentFps_ << std::endl;
					tempFps = 0;
				}
				
				for (auto& scene : scenes_)
				{
					scene->tick(deltaTime);
				}
				
				/*
				if ( sf::Keyboard::isKeyPressed(sf::Keyboard::N))
				{
					setState(GAME_STATE_START_NEW_GAME);
				}
				*/
				
				end = begin;
				break;
			
			case GAME_STATE_START_NEW_GAME:
				//sfmlWindow_->setMouseCursorVisible(false);
				graphicsEngine_->setMouseRelativeMode(true);
				graphicsEngine_->setCursorVisible(false);
				
				//camera_->tick(deltaTime);
				//igui_->processMessages();
				//igui_->update();

				handleEvents();
				
				startNewGame();
				
				setState(GAME_STATE_LOAD_NEW_GAME);
				
				break;
			
			case GAME_STATE_LOAD_NEW_GAME:
				{
					bool doneLoading = threadPool_->getActiveWorkerCount() == 0u && threadPool_->getWorkQueueCount() == 0u;
					doneLoading = doneLoading && openGlLoader_->getWorkQueueCount() == 0u;
					
					if (doneLoading)
					{
						setState(GAME_STATE_IN_GAME);
					}
					else
					{
						// Load any opengl assets (this needs work...)
						openGlLoader_->tick();
						//openGlLoader_->tick();
						
						//igui_->processMessages();
						
						if (temp > 200)
						{
							//igui_->update();
							temp = 0;
						}
						
						temp++;
					}
				}
				break;
	
			case GAME_STATE_IN_GAME:
				{
					//auto c = clock.getElapsedTime();
					handleEvents();
					//timeForInput_ = clock.getElapsedTime().asMilliseconds() - c.asMilliseconds();
				}
				
				{
					//physicsEngine_->tick(0.15f);
				}
			
				{
					//auto c = clock.getElapsedTime();
					
					// force mouse to center of window (so that we can use relative mouse movement for player movement)
					/*
					if ( !inConsole_ )
					{
						const sf::Vector2u windowSize = sfmlWindow_->getSize();
						glm::vec2 windowPosition = window_->getPosition();
						
						const sf::Vector2i mousePos = sf::Mouse::getPosition();
						const sf::Vector2i windowCenter = sf::Vector2i(windowPosition.x + windowSize.x / 2, windowPosition.y + windowSize.y / 2);
						sf::Mouse::setPosition(windowCenter);
						sfmlWindow_->setMouseCursorVisible(false);
						
						GameEngine::rotationX = (-1.0f) * (float32)(mousePos.x - windowCenter.x);
						GameEngine::rotationY = (-1.0f) * (float32)(mousePos.y - windowCenter.y);
						GameEngine::mousePosX = mousePos.x;
						GameEngine::mousePosY = mousePos.y;
					
						if ( sf::Keyboard::isKeyPressed(sf::Keyboard::W))
							camera_->moveForward();
			
						if ( sf::Keyboard::isKeyPressed(sf::Keyboard::S))
							camera_->moveBack();
			
						if ( sf::Keyboard::isKeyPressed(sf::Keyboard::A))
							camera_->moveLeft();
						
						if ( sf::Keyboard::isKeyPressed(sf::Keyboard::D))
							camera_->moveRight();
			
						if ( GameEngine::rotationX != 0.0f )
						{
							camera_->rotate( GameEngine::rotationX, glm::vec3(0.0f, 1.0f, 0.0f) );
							GameEngine::rotationX = 0.0f;
						}
			
						if ( GameEngine::rotationY != 0.0f )
						{
							camera_->rotate( GameEngine::rotationY, glm::vec3(1.0f, 0.0f, 0.0f) );
							GameEngine::rotationY = 0.0f;
						}
		
						camera_->tick(deltaTime);
					}
					else
					{
						sfmlWindow_->setMouseCursorVisible(true);
					}
					
					timeForMisc_ = clock.getElapsedTime().asMilliseconds() - c.asMilliseconds();
					*/
				}
				
				{
					// TODO: actually calculate the elapsed time
					//auto c = clock.getElapsedTime();
					//tick(deltaTime);
					//timeForTick_ = clock.getElapsedTime().asMilliseconds() - c.asMilliseconds();
				}
	
				break;
			
			default:
				std::string message = std::string("Game is in an unknown state!");
				logger_->error(message);
				assert(0);
				// TODO: Throw exception
				// throw exception::InvalidStateException(message);
				
				break;
		}

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
		
		if ( retVal == -1 )
		{
			running_ = false;
		}
		else
		{
			/*
			previousTime = currentTime;
			currentTime = clock.getElapsedTime();
			deltaTime = (float32)(currentTime.asMilliseconds() - previousTime.asMilliseconds()) / 1000.0f;
			tempFps++;

			if (currentTime.asMilliseconds() - previousFpsTime.asMilliseconds() > 1000.0f)
			{
				previousFpsTime = currentTime;
				currentFps_ = tempFps;
				tempFps = 0;
			}

			//igui_->processMessages();

			// update gui every 200 ms
			if ( currentTime.asMilliseconds() - lastGuiUpdateTime.asMilliseconds() > 200 )
			{
				auto c = clock.getElapsedTime();
				//igui_->update();
				lastGuiUpdateTime = currentTime;
				timeForGuiUpdate_ = clock.getElapsedTime().asMilliseconds() - c.asMilliseconds();
			}
			
			// run angelscript every 50 ms
			if ( currentTime.asMilliseconds() - lastAngelscriptUpdateTime.asMilliseconds() > 50 )
			{
				auto c = clock.getElapsedTime();
				lastAngelscriptUpdateTime = currentTime;
				angelscriptTest();
				timeForScripting_ = clock.getElapsedTime().asMilliseconds() - c.asMilliseconds();
				
			}
			
			// render the scene
			//openGlLoader_->block();
			auto c = clock.getElapsedTime();
			glrProgram_->render();
			timeForRender_ = clock.getElapsedTime().asMilliseconds() - c.asMilliseconds();
			//openGlLoader_->unblock();
			*/
			graphicsEngine_->render( 0.01f );
		}
		
		/*
		if ( sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			done = true;
		}
		*/
	}

	//igui_->release(mainGui_);
	
	//mainGui_ = nullptr;
	
	//igui_ = nullptr;

	//window_->destroy();

	//scriptingEngine_->destroy();

	destroy();
}

}
