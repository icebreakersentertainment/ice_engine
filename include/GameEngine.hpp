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
#include "DebugRenderer.hpp"
#include "IPluginManager.hpp"

#include "graphics/IGraphicsEngineFactory.hpp"
#include "graphics/IGraphicsEngine.hpp"
#include "graphics/IEventListener.hpp"

#include "pathfinding/IPathfindingEngineFactory.hpp"

#include "scripting/IScriptingEngine.hpp"

namespace ice_engine
{

class GameEngine : public IGameEngine, public graphics::IEventListener
{
public:
	GameEngine(
		std::unique_ptr<utilities::Properties> properties,
		std::unique_ptr<ice_engine::IPluginManager> pluginManager,
		std::unique_ptr<ice_engine::logger::ILogger> logger
	);
	GameEngine(
		std::unique_ptr<utilities::Properties> properties,
		std::unique_ptr<ice_engine::logger::ILogger> logger,
		std::unique_ptr<ice_engine::IPluginManager> pluginManager,
		std::unique_ptr<graphics::IGraphicsEngineFactory> graphicsEngineFactory
	);
	virtual ~GameEngine();

	virtual void run() override;
	
	virtual const EngineStatistics& getEngineStatistics() const override;
	
	virtual void setIGameInstance(scripting::ScriptObjectHandle scriptObjectHandle) override;

	/**
	 * 
	 */
	virtual void setBootstrapScript(const std::string& filename) override;
	
	virtual graphics::IGraphicsEngine* getGraphicsEngine() const override;
	virtual physics::IPhysicsEngine* getPhysicsEngine() const override;
	virtual IDebugRenderer* getDebugRenderer() const override;
	virtual pathfinding::IPathfindingEngine* getPathfindingEngine() const override;
	virtual IThreadPool* getBackgroundThreadPool() const override;
	virtual IThreadPool* getForegroundThreadPool() const override;
	virtual IOpenGlLoader* getOpenGlLoader() const override;
	
	virtual graphics::gui::IGui* createGui(const std::string& name) override;
	virtual void destroyGui(const graphics::gui::IGui* gui) override;
	
	virtual void setCallback(graphics::gui::IButton* button, scripting::ScriptFunctionHandle scriptFunctionHandle) override;
	
	//virtual AudioSample* loadAudioSample(const std::string& name, const std::string& filename) override;
	virtual image::Image* loadImage(const std::string& name, const std::string& filename) override;
	virtual std::shared_future<image::Image*> loadImageAsync(const std::string& name, const std::string& filename) override;
	virtual graphics::model::Model* loadModel(const std::string& name, const std::string& filename) override;
	virtual std::shared_future<graphics::model::Model*> loadModelAsync(const std::string& name, const std::string& filename) override;
	virtual graphics::model::Model* importModel(const std::string& name, const std::string& filename) override;
	virtual std::shared_future<graphics::model::Model*> importModelAsync(const std::string& name, const std::string& filename) override;
	
	//virtual void unloadAudioSample(const std::string& name) override;
	virtual void unloadImage(const std::string& name) override;
	virtual void unloadModel(const std::string& name) override;
	
	//virtual AudioSample* getAudioSample(const std::string& name) const
	virtual image::Image* getImage(const std::string& name) const override;
	virtual graphics::model::Model* getModel(const std::string& name) const override;
	
	virtual ModelHandle loadStaticModel(const graphics::model::Model* model) override;
	virtual std::shared_future<ModelHandle> loadStaticModelAsync(const graphics::model::Model* model) override;
	virtual graphics::RenderableHandle createRenderable(const graphics::RenderSceneHandle& renderSceneHandle, const ModelHandle& modelHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name = std::string()) override;
	virtual graphics::RenderableHandle createRenderable(const graphics::RenderSceneHandle& renderSceneHandle, const graphics::MeshHandle& meshHandle, const graphics::TextureHandle& textureHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name = std::string()) override;
	
	virtual graphics::VertexShaderHandle createVertexShader(const std::string& name, const std::string& filename) override;
	virtual std::shared_future<graphics::VertexShaderHandle> createVertexShaderAsync(const std::string& name, const std::string& filename) override;
	virtual graphics::VertexShaderHandle createVertexShaderFromSource(const std::string& name, const std::string& data) override;
	virtual std::shared_future<graphics::VertexShaderHandle> createVertexShaderFromSourceAsync(const std::string& name, const std::string& data) override;
	virtual graphics::FragmentShaderHandle createFragmentShader(const std::string& name, const std::string& filename) override;
	virtual std::shared_future<graphics::FragmentShaderHandle> createFragmentShaderAsync(const std::string& name, const std::string& filename) override;
	virtual graphics::FragmentShaderHandle createFragmentShaderFromSource(const std::string& name, const std::string& data) override;
	virtual std::shared_future<graphics::FragmentShaderHandle> createFragmentShaderFromSourceAsync(const std::string& name, const std::string& data) override;
	virtual graphics::VertexShaderHandle getVertexShader(const std::string& name) const override;
	virtual graphics::FragmentShaderHandle getFragmentShader(const std::string& name) const override;
	virtual void destroyShader(const std::string& name) override;
	virtual void destroyShader(const graphics::VertexShaderHandle& shaderHandle) override;
	virtual void destroyShader(const graphics::FragmentShaderHandle& shaderHandle) override;
	
	virtual graphics::ShaderProgramHandle createShaderProgram(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle) override;
	virtual std::shared_future<graphics::ShaderProgramHandle> createShaderProgramAsync(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle) override;
	virtual graphics::ShaderProgramHandle getShaderProgram(const std::string& name) const override;
	virtual void destroyShaderProgram(const std::string& name) override;
	virtual void destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle) override;
	
	virtual IScene* createScene(const std::string& name) override;
	virtual void destroyScene(const std::string& name) override;
	virtual void destroyScene(IScene* scene) override;
	virtual IScene* getScene(const std::string& name) const override;
	virtual std::vector<IScene*> getAllScenes() const override;
	
	virtual void addKeyboardEventListener(IKeyboardEventListener* keyboardEventListener) override;
	virtual void addMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener) override;
	virtual void addMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener) override;
	virtual void addMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener) override;
	virtual void removeKeyboardEventListener(IKeyboardEventListener* keyboardEventListener) override;
	virtual void removeMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener) override;
	virtual void removeMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener) override;
	virtual void removeMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener) override;
	
	void addKeyboardEventListener(scripting::ScriptObjectHandle keyboardEventListener);
	void addMouseMotionEventListener(scripting::ScriptObjectHandle mouseMotionEventListener);
	void addMouseButtonEventListener(scripting::ScriptObjectHandle mouseButtonEventListener);
	void addMouseWheelEventListener(scripting::ScriptObjectHandle mouseWheelEventListener);
	void removeKeyboardEventListener(scripting::ScriptObjectHandle keyboardEventListener);
	void removeMouseMotionEventListener(scripting::ScriptObjectHandle mouseMotionEventListener);
	void removeMouseButtonEventListener(scripting::ScriptObjectHandle mouseButtonEventListener);
	void removeMouseWheelEventListener(scripting::ScriptObjectHandle mouseWheelEventListener);
	
	// Implements the IEventListener interface
	virtual bool processEvent(const graphics::Event& event) override;

private:
	//Graphics
	std::unique_ptr<graphics::IGraphicsEngineFactory> graphicsEngineFactory_;
	std::unique_ptr< graphics::IGraphicsEngine > graphicsEngine_;
	std::vector<std::unique_ptr< graphics::gui::IGui >> guis_;
	
	std::unique_ptr<IDebugRenderer> debugRenderer_;
	
	
	std::unique_ptr< physics::IPhysicsEngine > physicsEngine_;
	std::unique_ptr<pathfinding::IPathfindingEngineFactory> pathfindingEngineFactory_;
	std::unique_ptr< pathfinding::IPathfindingEngine > pathfindingEngine_;
	std::unique_ptr<scripting::IScriptingEngine> scriptingEngine_;
	
	std::unique_ptr<ice_engine::IPluginManager> pluginManager_;
	
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
	
	std::map<std::string, graphics::VertexShaderHandle> vertexShaderHandles_;
	std::map<std::string, graphics::FragmentShaderHandle> fragmentShaderHandles_;
	std::map<std::string, graphics::ShaderProgramHandle> shaderProgramHandles_;
	
	scripting::ScriptHandle bootstrapScriptHandle_;
	scripting::ScriptObjectHandle scriptObjectHandle_;
	
	ResourceCache resourceCache_;
	
	bool running_;	
	EngineStatistics engineStatistics_;

	void tick(const float32 delta);
	void initialize();
	void destroy();
	void exit();

	void handleEvents();

	static float32 rotationX;
	static float32 rotationY;
	static int32 mousePosX;
	static int32 mousePosY;
	
	std::string bootstrapScriptName_;

	// Testing
	void test();
		
	// Initialization stuff
	void initializeLoggingSubSystem();
	void initializeFileSystemSubSystem();
	void initializeSoundSubSystem();
	void initializePhysicsSubSystem();
	void initializePathfindingSubSystem();
	void initializeGraphicsSubSystem();
	void initializeInputSubSystem();
	void initializeScriptingSubSystem();
	void initializeThreadingSubSystem();
	void initializeDataStoreSubSystem();
	void initializeEntitySubSystem();
	
	void loadEssentialGameData();
	void loadUserInterface();
	
	std::vector<std::pair<graphics::MeshHandle, graphics::TextureHandle>> staticModels_;
	
	// testing
	std::unique_ptr<ThreadPool> backgroundThreadPool_;
	std::unique_ptr<ThreadPool> foregroundThreadPool_;
	std::unique_ptr<OpenGlLoader> openGlLoader_;
	
	//std::unique_ptr<pyliteserializer::SqliteDataStore> dataStore_;
};

}

#endif /* GAMEENGINE_H_ */
