#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
//#include <SFML/Window.hpp>

#include "extras/FpsCamera.hpp"

#include "utilities/Properties.hpp"
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

namespace glmd = glm::detail;

namespace hercules
{

class GameEngine : public IGameEngine, public graphics::IEventListener
{
public:
	GameEngine();
	virtual ~GameEngine();

	virtual void run();

	virtual GameState getState();

	virtual entities::Entity* createEntity();
	virtual entities::Entity* createEntity(const std::string& name);
	virtual entities::Entity* getEntity(const std::string& name);
	
	/**
	 * 
	 */
	virtual void setBootstrapScript(const std::string& className, const std::string& filename);
	
	// Implements the IEventListener interface
	virtual bool processEvent(const graphics::Event& event);

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
	
	std::unique_ptr<Player> player_;
	
	std::unique_ptr<world::World> world_;
	
	// For some reason, AngelScript will crash with a SegFault when registering objects if I don't use a pointer here (instead of storing it on the stack)
	std::unique_ptr<as_wrapper::AngelScript> angelScript_;
	
	std::unique_ptr<fs::IFileSystem> fileSystem_;
	
	bool running_;
	GameState state_;

	void tick(glmd::float32 elapsedTime);
	void initialize();
	void destroy();
	void exit();

	void setState(GameState state);

	void startNewGame();

	void handleEvents();

	static glm::detail::float32 rotationX;
	static glm::detail::float32 rotationY;
	static glm::detail::int32 mousePosX;
	static glm::detail::int32 mousePosY;

	// Testing
	void test();
	
	void angelscriptTest();
	as_wrapper::AsObject* mainAsScript_;

	glm::detail::float32 currentFps_;
	glm::detail::float32 getFps();
	glm::detail::uint32 getThreadPoolQueueSize();
	glm::detail::uint32 getOpenGlThreadPoolQueueSize();
	glm::detail::uint32 getThreadPoolWorkerCount();
	glm::detail::uint32 getThreadPoolActiveWorkerCount();
	glm::detail::uint32 getThreadPoolInactiveWorkerCount();
	
	// Keeping track of how long things take
	glmd::int32 timeForPhysics_;
	glmd::int32 timeForTick_;
	glmd::int32 timeForRender_;
	glmd::int32 timeForMisc_;
	glmd::int32 timeForInput_;
	glmd::int32 timeForGuiUpdate_;
	glmd::int32 timeForAngelScript_;
	
	glm::detail::int32 getTimeForPhysics();
	glm::detail::int32 getTimeForTick();
	glm::detail::int32 getTimeForRender();
	glm::detail::int32 getTimeForMisc();
	glm::detail::int32 getTimeForInput();
	glm::detail::int32 getTimeForGuiUpdate();
	glm::detail::int32 getTimeForAngelScript();
	
	// Initialization stuff
	void initializeLoggingSubSystem();
	void loadProperties();
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
	
	static glmd::uint32 COMPONENT_TYPE_GRAPHICS;
	
	// testing
	std::unique_ptr<ThreadPool> threadPool_;
	std::unique_ptr<OpenGlLoader> openGlLoader_;
	
	//std::unique_ptr<pyliteserializer::SqliteDataStore> dataStore_;
	
	// test
	bool inConsole_;
};

}

#endif /* GAMEENGINE_H_ */
