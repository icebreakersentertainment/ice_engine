#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <vector>
#include <map>
#include <utility>
#include <memory>

#include "extras/FpsCamera.hpp"

#include "utilities/Properties.hpp"
#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "IGameEngine.hpp"
#include "ResourceCache.hpp"
#include "Camera.hpp"
#include "ThreadPool.hpp"
#include "OpenGlLoader.hpp"

#include "graphics/IGraphicsEngine.hpp"
#include "graphics/IEventListener.hpp"

#include "scripting/IScriptingEngine.hpp"

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

	virtual void run() override;

	virtual GameState getState() override;
	
	virtual void setIGameInstance(asIScriptObject* obj) override;

	/**
	 * 
	 */
	virtual void setBootstrapScript(const std::string& filename) override;
	
	virtual graphics::IGraphicsEngine* getGraphicsEngine() const override;
	virtual physics::IPhysicsEngine* getPhysicsEngine() const override;
	
	//virtual AudioSample* loadAudioSample(const std::string& name, const std::string& filename) override;
	virtual image::Image* loadImage(const std::string& name, const std::string& filename) override;
	virtual graphics::model::Model* loadModel(const std::string& name, const std::string& filename) override;
	virtual graphics::model::Model* importModel(const std::string& name, const std::string& filename) override;
	
	//virtual void unloadAudioSample(const std::string& name) override;
	virtual void unloadImage(const std::string& name) override;
	virtual void unloadModel(const std::string& name) override;
	
	//virtual AudioSample* getAudioSample(const std::string& name) const
	virtual image::Image* getImage(const std::string& name) const override;
	virtual graphics::model::Model* getModel(const std::string& name) const override;
	
	virtual ModelHandle loadStaticModel(const graphics::model::Model* model) override;
	virtual graphics::RenderableHandle createRenderable(const ModelHandle& modelHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name = std::string()) override;
	
	virtual graphics::ShaderHandle createVertexShader(const std::string& name, const std::string& filename) override;
	virtual graphics::ShaderHandle createVertexShaderFromSource(const std::string& name, const std::string& data) override;
	virtual graphics::ShaderHandle createFragmentShader(const std::string& name, const std::string& filename) override;
	virtual graphics::ShaderHandle createFragmentShaderFromSource(const std::string& name, const std::string& data) override;
	virtual graphics::ShaderHandle getShader(const std::string& name) const override;
	virtual void destroyShader(const std::string& name) override;
	virtual void destroyShader(const graphics::ShaderHandle& shaderHandle) override;
	
	virtual graphics::ShaderProgramHandle createShaderProgram(const std::string& name, const graphics::ShaderHandle& vertexShaderHandle, const graphics::ShaderHandle& fragmentShaderHandle) override;
	virtual graphics::ShaderProgramHandle getShaderProgram(const std::string& name) const override;
	virtual void destroyShaderProgram(const std::string& name) override;
	virtual void destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle) override;
	
	virtual IScene* createScene(const std::string& name) override;
	virtual void destroyScene(const std::string& name) override;
	virtual void destroyScene(IScene* scene) override;
	virtual IScene* getScene(const std::string& name) const override;
	
	virtual void addKeyboardEventListener(IKeyboardEventListener* keyboardEventListener) override;
	virtual void addMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener) override;
	virtual void addMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener) override;
	virtual void addMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener) override;
	virtual void removeKeyboardEventListener(IKeyboardEventListener* keyboardEventListener) override;
	virtual void removeMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener) override;
	virtual void removeMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener) override;
	virtual void removeMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener) override;
	
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
	
	std::unique_ptr< physics::IPhysicsEngine > physicsEngine_;
	std::unique_ptr<scripting::IScriptingEngine> scriptingEngine_;
	
	std::vector<IKeyboardEventListener*> keyboardEventListeners_;
	std::vector<IMouseMotionEventListener*> mouseMotionEventListeners_;
	std::vector<IMouseButtonEventListener*> mouseButtonEventListeners_;
	std::vector<IMouseWheelEventListener*> mouseWheelEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptKeyboardEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseMotionEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseButtonEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseWheelEventListeners_;
	
	std::vector<std::unique_ptr<IScene>> scenes_;
	
	std::unique_ptr< utilities::Properties > properties_;
	std::unique_ptr< logger::ILogger > logger_;
	std::unique_ptr<fs::IFileSystem> fileSystem_;
	
	std::map<std::string, graphics::ShaderHandle> shaderHandles_;
	std::map<std::string, graphics::ShaderProgramHandle> shaderProgramHandles_;
	
	ResourceCache resourceCache_;
	
	bool running_;
	GameState state_;

	void tick(const float32 delta);
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
	
	void addKeyboardEventListener(asIScriptObject* keyboardEventListener);
	void addMouseMotionEventListener(asIScriptObject* mouseMotionEventListener);
	void addMouseButtonEventListener(asIScriptObject* mouseButtonEventListener);
	void addMouseWheelEventListener(asIScriptObject* mouseWheelEventListener);
	void removeKeyboardEventListener(asIScriptObject* keyboardEventListener);
	void removeMouseMotionEventListener(asIScriptObject* mouseMotionEventListener);
	void removeMouseButtonEventListener(asIScriptObject* mouseButtonEventListener);
	void removeMouseWheelEventListener(asIScriptObject* mouseWheelEventListener);
	
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
	
	static uint32 COMPONENT_TYPE_GRAPHICS;
	
	std::vector<std::pair<graphics::MeshHandle, graphics::TextureHandle>> staticModels_;
	
	// testing
	std::unique_ptr<ThreadPool> threadPool_;
	std::unique_ptr<OpenGlLoader> openGlLoader_;
	
	//std::unique_ptr<pyliteserializer::SqliteDataStore> dataStore_;
	
	// test
	bool inConsole_;
};

}

#endif /* GAMEENGINE_H_ */
