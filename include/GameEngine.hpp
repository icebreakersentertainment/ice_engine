#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <memory>

#include <entityx/entityx.h>

#include "extras/FpsCamera.hpp"

#include "utilities/Properties.hpp"
#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "IGameEngine.hpp"
#include "Player.hpp"
#include "Camera.hpp"
#include "ThreadPool.hpp"
#include "OpenGlLoader.hpp"

#include "graphics/IGraphicsEngine.hpp"
#include "graphics/IEventListener.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "entities/Entity.hpp"
#include "entities/GraphicsComponent.hpp"

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

	virtual entities::Entity createEntity() override;
	
	virtual void assign(const entities::Entity entity, const entities::GraphicsComponent& component) override;
	
	virtual void rotate(const entities::Entity entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) override;
	virtual void translate(const entities::Entity entity, const glm::vec3& translate) override;
	virtual void setScale(const entities::Entity entity, const glm::vec3& scale) override;
	virtual void setScale(const entities::Entity entity, const float32 x, const float32 y, const float32 z) override;
	virtual void lookAt(const entities::Entity entity, const glm::vec3& lookAt) override;
	
	virtual void setPosition(const entities::Entity entity, const glm::vec3& position) override;
	virtual void setPosition(const entities::Entity entity, const float32 x, const float32 y, const float32 z) override;
	
	/**
	 * 
	 */
	virtual void setBootstrapScript(const std::string& filename) override;
	
	virtual model::Model importModel(const std::string& filename, const std::string& name = std::string()) const override;
	virtual graphics::ModelHandle loadModel(const model::Model& model) override;
	
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
	graphics::CameraHandle cameraHandle_;
	
	std::unique_ptr< physics::IPhysicsEngine > physicsEngine_;
	
	std::unique_ptr< utilities::Properties > properties_;
	std::unique_ptr< logger::ILogger > logger_;
	
	std::unique_ptr<Player> player_;
	
	std::unique_ptr<scripting::IScriptingEngine> scriptingEngine_;
	
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
	
	std::string bootstrapScriptName_;

	// Testing
	void test();

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
	int32 timeForScripting_;
	
	int32 getTimeForPhysics();
	int32 getTimeForTick();
	int32 getTimeForRender();
	int32 getTimeForMisc();
	int32 getTimeForInput();
	int32 getTimeForGuiUpdate();
	int32 getTimeForScripting();
	
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
	std::vector<entities::Entity> entities_;
	
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
