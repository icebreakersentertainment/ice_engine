#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <memory>

#include "extras/FpsCamera.hpp"

#include "utilities/Properties.hpp"
#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "IGameEngine.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "ThreadPool.hpp"
#include "OpenGlLoader.hpp"
#include "world/World.hpp"

#include "graphics/IGraphicsEngine.hpp"
#include "graphics/IEventListener.hpp"

#include "physics/IPhysicsEngine.hpp"

//#include "data_store/SqliteDataStore.hpp"

#include "entities/EntityFactory.hpp"
#include "entities/GraphicsComponentFactory.hpp"

namespace hercules
{

class GameEngine : public IGameEngine, public graphics::IEventListener
{
public:
	GameEngine(
		std::unique_ptr<utilities::Properties> properties,
		std::unique_ptr<hercules::logger::ILogger> logger
	);
	virtual ~GameEngine();

	virtual void run();

	virtual GameState getState() override;

	virtual entities::Entity* createEntity() override;
	virtual entities::Entity* createEntity(const std::string& name) override;
	virtual entities::Entity* getEntity(const std::string& name) override;
	
	/**
	 * 
	 */
	virtual void setBootstrapScript(const std::string& className, const std::string& filename) override;
	
	// Implements the IEventListener interface
	virtual bool processEvent(const graphics::Event& event) override;

private:
	//std::unique_ptr< glr::GlrProgram > glrProgram_;
	//glr::IWindow* window_;
	//sf::Window* sfmlWindow_;
	//glr::gui::IGui* igui_;
	//glr::ISceneManager* smgr_;
	//std::unique_ptr< Camera > camera_;
	
	// Gui
	//glr::gui::IGuiComponent* mainGui_;
	std::unique_ptr< graphics::IGraphicsEngine > graphicsEngine_;
	graphics::CameraId cameraId_;
	
	std::unique_ptr< physics::IPhysicsEngine > physicsEngine_;
	
	std::unique_ptr< utilities::Properties > properties_;
	std::unique_ptr< logger::ILogger > logger_;
	
	std::unique_ptr<Player> player_;
	
	std::unique_ptr<world::World> world_;
	
	// For some reason, AngelScript will crash with a SegFault when registering objects if I don't use a pointer here (instead of storing it on the stack)
	std::unique_ptr<as_wrapper::AngelScript> angelScript_;
	
	std::unique_ptr<fs::IFileSystem> fileSystem_;
	
	bool running_;
	GameState state_;

	void tick(float32 elapsedTime);
	void initialize();
	void destroy();
	void exit();

	void setState(GameState state);

	void startNewGame();

	void handleEvents();

	static float32 rotationX;
	static float32 rotationY;
	static int32 mousePosX;
	static int32 mousePosY;

	// Testing
	void test();
	
	void angelscriptTest();
	as_wrapper::AsObject* mainAsScript_;

	float32 currentFps_;
	float32 getFps();
	uint32 getThreadPoolQueueSize();
	uint32 getOpenGlThreadPoolQueueSize();
	uint32 getThreadPoolWorkerCount();
	uint32 getThreadPoolActiveWorkerCount();
	uint32 getThreadPoolInactiveWorkerCount();
	
	// Keeping track of how long things take
	int32 timeForPhysics_;
	int32 timeForTick_;
	int32 timeForRender_;
	int32 timeForMisc_;
	int32 timeForInput_;
	int32 timeForGuiUpdate_;
	int32 timeForAngelScript_;
	
	int32 getTimeForPhysics();
	int32 getTimeForTick();
	int32 getTimeForRender();
	int32 getTimeForMisc();
	int32 getTimeForInput();
	int32 getTimeForGuiUpdate();
	int32 getTimeForAngelScript();
	
	// Initialization stuff
	void initializeLoggingSubSystem();
	void initializeFileSystemSubSystem();
	void initializeSoundSubSystem();
	void initializePhysicsSubSystem();
	void initializeGraphicsSubSystem();
	void initializeInputSubSystem();
	void initializeScriptingSubSystem();
	void initializeThreadingSubSystem();
	void initializeDataStoreSubSystem();
	void initializeEntitySubSystem();
	
	void loadEssentialGameData();
	void loadUserInterface();
	
	// Entity system
	entityx::EntityX entityx_;
	
	static uint32 COMPONENT_TYPE_GRAPHICS;
	
	// testing
	std::unique_ptr<ThreadPool> threadPool_;
	std::unique_ptr<OpenGlLoader> openGlLoader_;
	
	//std::unique_ptr<pyliteserializer::SqliteDataStore> dataStore_;
	
	// test
	bool inConsole_;
};

}

#endif /* GAMEENGINE_H_ */
