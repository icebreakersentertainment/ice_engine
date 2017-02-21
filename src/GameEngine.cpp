#include <iostream>
#include <sstream>
#include <utility>

//#include <boost/filesystem.hpp>

#include "GameEngine.hpp"

#include "Constants.hpp"

#include "graphics/GraphicsFactory.hpp"
#include "graphics/Event.hpp"

#include "model/ModelLoader.hpp"
#include "model/Animate.hpp"

#include "physics/PhysicsFactory.hpp"

#include "logger/Logger.hpp"

#include "as_wrapper/AngelScript.hpp"

#include "angel_script/TestAtom.hpp"
#include "angel_script/ClassFactory.hpp"

#include "fs/FileSystem.hpp"

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
	
	inConsole_ = false;
	
	mainAsScript_ = nullptr;
	
	initialize();
}

GameEngine::~GameEngine()
{
	logger_->info("Shutting down.");
	
	if (mainAsScript_ != nullptr)
	{
		mainAsScript_->callMethod( std::string("void destroy()") );
	}
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
	// Testing angelscript
	if (mainAsScript_ != nullptr)
		mainAsScript_->callMethod( std::string("void tick()") );
	
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
	logger_->info( "initialize physics." );
	
	physicsEngine_ = physics::PhysicsFactory::createPhysicsEngine();
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
	logger_->info( "Initializing angelscript..." );
	// TODO: This is a bit hacky
	//entities::GraphicsComponentFactory::sceneManager_ = smgr_;
	
	logger_->debug( "create angelscript wrapper." );

	angelScript_ = std::unique_ptr<as_wrapper::AngelScript>( new as_wrapper::AngelScript(logger_.get()) );
	
	// Register Classes available to scripts
	angelScript_->registerClass(
		std::string("Entity"),
		std::string("Entity@ f()"),
		std::string("void f()"),
		std::string("void f()"),
		asFUNCTION(entities::EntityFactory::entityRefFactory),
		asMETHOD(entities::Entity, addRef),
		asMETHOD(entities::Entity, releaseRef)
	);
	
	angelScript_->registerClass(
		std::string("GraphicsComponent"),
		std::string("GraphicsComponent@ f()"),
		std::string("void f()"),
		std::string("void f()"),
		asFUNCTION(entities::GraphicsComponentFactory::graphicsComponentRefFactory),
		asMETHOD(entities::GraphicsComponent, addRef),
		asMETHOD(entities::GraphicsComponent, releaseRef)
	);
	
	angelScript_->registerClassMethod(std::string("Entity"), std::string("void addComponent(GraphicsComponent@)"), asMETHODPR(entities::Entity, addComponent, (entities::GraphicsComponent*), void));
	
	angelScript_->registerClassMethod(std::string("GraphicsComponent"), std::string("void rotate(float, const vec3 &in)"), asMETHODPR(entities::GraphicsComponent, rotate, (float, const Vec3 &), void));
	angelScript_->registerClassMethod(std::string("GraphicsComponent"), std::string("void translate(const vec3 &in)"), asMETHODPR(entities::GraphicsComponent, translate, (const Vec3 &), void));
	angelScript_->registerClassMethod(std::string("GraphicsComponent"), std::string("void setScale(float, float, float)"), asMETHODPR(entities::GraphicsComponent, setScale, (float, float, float), void));
	angelScript_->registerClassMethod(std::string("GraphicsComponent"), std::string("void lookAt(const vec3 &in)"), asMETHODPR(entities::GraphicsComponent, lookAt, (const Vec3 &), void));
	angelScript_->registerClassMethod(std::string("GraphicsComponent"), std::string("void setPosition(const vec3 &in)"), asMETHODPR(entities::GraphicsComponent, setPosition, (const Vec3 &), void));
	angelScript_->registerClassMethod(std::string("GraphicsComponent"), std::string("void setPosition(float, float, float)"), asMETHODPR(entities::GraphicsComponent, setPosition, (float, float, float), void));
	
	// Global functions
	angelScript_->registerGlobalFunction(
		std::string("Entity@ getEntity(const string& in)"),
		asMETHODPR(IGameEngine, getEntity, (const std::string&), entities::Entity*),
		asCALL_THISCALL_ASGLOBAL,
		this
	);
	
	angelScript_->registerGlobalFunction(
		std::string("Entity@ createEntity()"),
		asMETHODPR(IGameEngine, createEntity, (), entities::Entity*),
		asCALL_THISCALL_ASGLOBAL,
		this
	);
	
	// Set global constants
	angelScript_->registerGlobalProperty(std::string("const uint COMPONENT_TYPE_GRAPHICS"), &GameEngine::COMPONENT_TYPE_GRAPHICS);
	
	// TESTING - loading scripts and creating objects
	angelScript_->loadScript(std::string("Main"), std::string("Main.as"));
	
	auto asObject = angelScript_->createAsObject(std::string("Main"), std::string("Main"));
	
	asObject->callMethod( std::string("void tick()") );
	
	/*
	angelScript_ = std::unique_ptr<as_wrapper::AngelScript>( new as_wrapper::AngelScript() );
	
	bool success = true;
	success = success && angelScript_->registerClass(
		std::string("TestAtom"), 
		std::string("TestAtom@ f()"), 
		std::string("void f()"), 
		std::string("void f()"), 
		asFUNCTION(angel_script::ClassFactory::atomRefFactory), 
		asMETHOD(angel_script::TestAtom, addRef), 
		asMETHOD(angel_script::TestAtom, releaseRef)
	);
	
	success = success && angelScript_->registerClassMethod(std::string("TestAtom"), std::string("void test()"), asMETHOD(angel_script::TestAtom, test));
	
	
	
	if ( !success )
	{
		logger_->warn( "Warning: Not all object/method registration completed successfully!" );
	}

	// load all of the scripts
	angelScript_->loadScripts();

	// testing
	angelScript_->initContext( std::string("void eat(TestAtom atom)"), std::string("test"));
	angel_script::TestAtom* atom = new angel_script::TestAtom();
	angelScript_->setArgObject(0, atom);
	delete atom;

	//angelScript_->initContext(text, "test");

	int r = angelScript_->run();
	angelScript_->releaseContext();
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
	logger_->info( "Load entity system..." );
	//entityEvents_ = entityx::ptr<entityx::EventManager>(new entityx::EventManager());
	//entities_ = entityx::ptr<entityx::EntityManager>(new entityx::EntityManager(entityEvents_));
}

std::vector<model::BoneData> boneData;
std::vector<model::Animation> animations;
model::BoneNode rootBoneNode;
glm::mat4 globalInverseTransformation;
graphics::SkeletonId skeletonId;
void GameEngine::test()
{
	cameraId_ = graphicsEngine_->createCamera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	std::vector<glm::vec3> vertices;
	
	vertices.push_back( glm::vec3(0.5f,  0.5f, 0.0f) );
	vertices.push_back( glm::vec3(0.5f, -0.5f, 0.0f) );
	vertices.push_back( glm::vec3(-0.5f, -0.5f, 0.0f) );
	vertices.push_back( glm::vec3(-0.5f,  0.5f, 0.0f) );
	
	std::vector<uint32> indices;
	// First triangle
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	// Second triangle
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	
	std::vector<glm::vec4> colors;
	
	colors.push_back( glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) );
	colors.push_back( glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) );
	colors.push_back( glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) );
	colors.push_back( glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) );
	
	std::vector<glm::vec3> normals;
	
	normals.push_back( glm::vec3(0.5f,  0.5f, 0.0f) );
	normals.push_back( glm::vec3(0.5f, -0.5f, 0.0f) );
	normals.push_back( glm::vec3(-0.5f, -0.5f, 0.0f) );
	normals.push_back( glm::vec3(-0.5f,  0.5f, 0.0f) );
	
	std::vector<glm::vec2> textureCoordinates;
	
	textureCoordinates.push_back( glm::vec2(1.0f, 1.0f) );
	textureCoordinates.push_back( glm::vec2(1.0f, 0.0f) );
	textureCoordinates.push_back( glm::vec2(0.0f, 0.0f) );
	textureCoordinates.push_back( glm::vec2(0.0f, 1.0f) );
	
	/*
	{
		auto meshId = graphicsEngine_->createStaticMesh(vertices, indices, colors, normals, textureCoordinates);
		auto textureId = graphicsEngine_->createTexture2d( std::string("../../assets/textures/ts_grass.jpg") );
		
		auto renderableId = graphicsEngine_->createRenderable(meshId, textureId);
	}
	*/
	
	/*
	{
		auto model = model::import(std::string("test_model"), std::string("../assets/models/scoutship/scoutship.dae"), logger_.get());
		
		std::cout << model->meshes.size() << " meshes." << std::endl;
		
		auto boneIds = std::vector< glm::ivec4 >();
		auto boneWeights = std::vector< glm::vec4 >();
		
		auto vertexBoneData = model->meshes[0].bones;
		
		for ( const auto& bd : vertexBoneData )
		{
			boneIds.push_back( bd.boneIds );
			boneWeights.push_back( bd.weights );
		}
		
		auto meshId = graphicsEngine_->createAnimatedMesh(model->meshes[0].vertices, model->meshes[0].indices, model->meshes[0].colors, model->meshes[0].normals, model->meshes[0].textureCoordinates, boneIds, boneWeights);
		std::cout << "Created animated mesh" << std::endl;
		auto textureId = graphicsEngine_->createTexture2d( std::string("../assets/models/scoutship/") + model->textures[0].filename );
		std::cout << "Created texture" << std::endl;
		auto renderableId = graphicsEngine_->createRenderable(meshId, textureId);
		std::cout << "Created renderable" << std::endl;
		graphicsEngine_->scale(renderableId, 0.03f);
		graphicsEngine_->translate(renderableId, 6.0f, -4.0f, 0);
		
		animations = model->animations;
		rootBoneNode = model->rootBoneNode;
		globalInverseTransformation = model->globalInverseTransformation;
		boneData = model->boneData;
		
		skeletonId = graphicsEngine_->createSkeleton( 100 );
		std::cout << "Created skeleton" << std::endl;
		graphicsEngine_->assign(renderableId, skeletonId);
	}
	*/
	
	{
		auto model = model::import(std::string("test_model"), std::string("../assets/models/scoutship/scoutship.dae"), logger_.get());
		auto meshId = graphicsEngine_->createStaticMesh(model->meshes[0].vertices, model->meshes[0].indices, model->meshes[0].colors, model->meshes[0].normals, model->meshes[0].textureCoordinates);
		auto textureId = graphicsEngine_->createTexture2d( std::string("../assets/models/scoutship/") + model->textures[0].filename );
		auto renderableId = graphicsEngine_->createRenderable(meshId, textureId);
		
		graphicsEngine_->scale(renderableId, 0.03f);
		graphicsEngine_->translate(renderableId, 6.0f, -4.0f, 0);
	}
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
		function = [this]() { return this->getTimeForAngelScript(); };
		gameGuiObject->addFunction(std::wstring(L"getTimeForAngelScript"), function);
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

entities::Entity* GameEngine::createEntity()
{
	return new entities::Entity( entityx_.entities.create() );
}

// TODO: Do we need a name?
entities::Entity* GameEngine::createEntity(const std::string& name)
{
	//return new Entity( name );
	return nullptr;
}

entities::Entity* GameEngine::getEntity(const std::string& name)
{
	//std::cout << "getting entity with name '" << name << "'." << std::endl;
	//glr::ISceneNode* node = smgr_->getSceneNode( name );
	//return new Entity( node );
	return nullptr;
}

void GameEngine::setBootstrapScript(const std::string& className, const std::string& filename)
{
	// Error check
	if (mainAsScript_ != nullptr)
	{
		// TODO: Throw exception
		assert(0);
	}
	
	angelScript_->loadScript(std::string("Main"), filename);
	
	mainAsScript_ = angelScript_->createAsObject(std::string("Main"), className);
	
	mainAsScript_->callMethod( std::string("void initialize()") );
}

void GameEngine::angelscriptTest()
{
	angelScript_->loadScript( std::string("manipulate_scene_nodes"), std::string("manipulate_scene_nodes.as") );
	
	angelScript_->initContext( std::string("void main()"), std::string("manipulate_scene_nodes"));

	angelScript_->run();
		
	angelScript_->releaseContext();
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
				graphicsEngine_->rotate(cameraId_, -degrees, glm::vec3(0.0f, 1.0f, 0.0f), graphics::TransformSpace::TS_LOCAL);
			}
			{
				const auto degrees = event.motion.yrel / 5.5f;
				graphicsEngine_->rotate(cameraId_, -degrees, glm::vec3(1.0f, 0.0f, 0.0f), graphics::TransformSpace::TS_WORLD);
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

int32 GameEngine::getTimeForAngelScript()
{
	return timeForAngelScript_;
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
	timeForAngelScript_ = 0;
	
	test();
	
	logger_->info( "We have liftoff..." );
	// start our clock
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
	
	while ( running_ )
	{
		switch ( state_ )
		{
			case GAME_STATE_MAIN_MENU:
				//sfmlWindow_->setMouseCursorVisible(true);
				graphicsEngine_->setMouseRelativeMode(false);
				graphicsEngine_->setCursorVisible(true);
				//camera_->tick(deltaTime);
	
				handleEvents();
				
				/*
				if ( sf::Keyboard::isKeyPressed(sf::Keyboard::N))
				{
					setState(GAME_STATE_START_NEW_GAME);
				}
				*/
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
					physicsEngine_->tick(0.15f);
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
		model::animateSkeleton(transformations, globalInverseTransformation, animatedBoneNodes, rootBoneNode, boneData[0], duration, ticksPerSecond, runningTime, 0, 0);
		graphicsEngine_->update(skeletonId, &transformations[0], 100 * sizeof(glm::mat4));
		*/

		transformations = std::vector< glm::mat4 >(100, glm::mat4(1.0));
		graphicsEngine_->update(skeletonId, &transformations[0], 100 * sizeof(glm::mat4));
		
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
				timeForAngelScript_ = clock.getElapsedTime().asMilliseconds() - c.asMilliseconds();
				
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

	//angelScript_->destroy();

	destroy();
}

}
