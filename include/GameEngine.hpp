#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <vector>
#include <map>
#include <utility>
#include <memory>
#include <future>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "EngineStatistics.hpp"
#include "IThreadPool.hpp"
#include "IOpenGlLoader.hpp"
#include "ModelHandle.hpp"
#include "IDebugRenderer.hpp"
//#include "Scene.hpp"
#include "IWindowEventListener.hpp"
#include "IKeyboardEventListener.hpp"
#include "IMouseMotionEventListener.hpp"
#include "IMouseButtonEventListener.hpp"
#include "IMouseWheelEventListener.hpp"
#include "IConnectEventListener.hpp"
#include "IDisconnectEventListener.hpp"
#include "IMessageEventListener.hpp"

#include "graphics/model/Model.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "image/Image.hpp"

#include "extras/FpsCamera.hpp"

#include "utilities/Properties.hpp"
#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "ResourceCache.hpp"
#include "Camera.hpp"
#include "ThreadPool.hpp"
#include "OpenGlLoader.hpp"
#include "DebugRenderer.hpp"
#include "IPluginManager.hpp"

#include "graphics/IGraphicsEngineFactory.hpp"
#include "graphics/IGraphicsEngine.hpp"
#include "graphics/IEventListener.hpp"
#include "graphics/gui/IGui.hpp"

#include "ITerrainFactory.hpp"

#include "audio/IAudioEngineFactory.hpp"
#include "audio/IAudioEngine.hpp"

#include "networking/INetworkingEngineFactory.hpp"
#include "networking/INetworkingEngine.hpp"
#include "networking/IEventListener.hpp"

#include "pathfinding/IPathfindingEngineFactory.hpp"
#include "pathfinding/IPathfindingEngine.hpp"

#include "scripting/IScriptingEngine.hpp"
#include "Audio.hpp"
namespace ice_engine
{

class Scene;

class GameEngine : public graphics::IEventListener, public networking::IEventListener
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
		std::unique_ptr<graphics::IGraphicsEngineFactory> graphicsEngineFactory,
		std::unique_ptr<ITerrainFactory> terrainFactory
	);
	virtual ~GameEngine();

	void run();
	
	const EngineStatistics& getEngineStatistics() const;
	
	void setIGameInstance(scripting::ScriptObjectHandle scriptObjectHandle);

	/**
	 * 
	 */
	void setBootstrapScript(const std::string& filename);
	
	audio::IAudioEngine* getAudioEngine() const;
	graphics::IGraphicsEngine* getGraphicsEngine() const;
	physics::IPhysicsEngine* getPhysicsEngine() const;
	scripting::IScriptingEngine* getScriptingEngine() const;
	IDebugRenderer* getDebugRenderer() const;
	pathfinding::IPathfindingEngine* getPathfindingEngine() const;
	IThreadPool* getBackgroundThreadPool() const;
	IThreadPool* getForegroundThreadPool() const;
	IOpenGlLoader* getOpenGlLoader() const;
	logger::ILogger* getLogger() const;
	fs::IFileSystem* getFileSystem() const;
	
	graphics::gui::IGui* createGui(const std::string& name);
	void destroyGui(const graphics::gui::IGui* gui);
	
	void setCallback(graphics::gui::IButton* button, scripting::ScriptFunctionHandle scriptFunctionHandle);
	void setCallback(graphics::gui::IMenuItem* menuItem, scripting::ScriptFunctionHandle scriptFunctionHandle);
	
	Audio* loadAudio(const std::string& name, const std::string& filename);
	std::shared_future<Audio*> loadAudioAsync(const std::string& name, const std::string& filename);
	Image* loadImage(const std::string& name, const std::string& filename);
	std::shared_future<Image*> loadImageAsync(const std::string& name, const std::string& filename);
	graphics::model::Model* loadModel(const std::string& name, const std::string& filename);
	std::shared_future<graphics::model::Model*> loadModelAsync(const std::string& name, const std::string& filename);
	graphics::model::Model* importModel(const std::string& name, const std::string& filename);
	std::shared_future<graphics::model::Model*> importModelAsync(const std::string& name, const std::string& filename);
	
	void unloadAudio(const std::string& name);
	void unloadImage(const std::string& name);
	void unloadModel(const std::string& name);
	
	Audio* getAudio(const std::string& name) const;
	Image* getImage(const std::string& name) const;
	graphics::model::Model* getModel(const std::string& name) const;
	
	ModelHandle loadStaticModel(const graphics::model::Model* model);
	std::shared_future<ModelHandle> loadStaticModelAsync(const graphics::model::Model* model);
	graphics::RenderableHandle createRenderable(const graphics::RenderSceneHandle& renderSceneHandle, const ModelHandle& modelHandle, const std::string& name = std::string());
	graphics::RenderableHandle createRenderable(const graphics::RenderSceneHandle& renderSceneHandle, const graphics::MeshHandle& meshHandle, const graphics::TextureHandle& textureHandle, const std::string& name = std::string());
	
	graphics::VertexShaderHandle createVertexShader(const std::string& name, const std::string& filename);
	std::shared_future<graphics::VertexShaderHandle> createVertexShaderAsync(const std::string& name, const std::string& filename);
	graphics::VertexShaderHandle createVertexShaderFromSource(const std::string& name, const std::string& data);
	std::shared_future<graphics::VertexShaderHandle> createVertexShaderFromSourceAsync(const std::string& name, const std::string& data);
	graphics::FragmentShaderHandle createFragmentShader(const std::string& name, const std::string& filename);
	std::shared_future<graphics::FragmentShaderHandle> createFragmentShaderAsync(const std::string& name, const std::string& filename);
	graphics::FragmentShaderHandle createFragmentShaderFromSource(const std::string& name, const std::string& data);
	std::shared_future<graphics::FragmentShaderHandle> createFragmentShaderFromSourceAsync(const std::string& name, const std::string& data);
	graphics::VertexShaderHandle getVertexShader(const std::string& name) const;
	graphics::FragmentShaderHandle getFragmentShader(const std::string& name) const;
	void destroyShader(const std::string& name);
	void destroyShader(const graphics::VertexShaderHandle& shaderHandle);
	void destroyShader(const graphics::FragmentShaderHandle& shaderHandle);
	
	graphics::ShaderProgramHandle createShaderProgram(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle);
	std::shared_future<graphics::ShaderProgramHandle> createShaderProgramAsync(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle);
	graphics::ShaderProgramHandle getShaderProgram(const std::string& name) const;
	void destroyShaderProgram(const std::string& name);
	void destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle);
	
	Scene* createScene(const std::string& name);
	void destroyScene(const std::string& name);
	void destroyScene(Scene* scene);
	Scene* getScene(const std::string& name) const;
	std::vector<Scene*> getAllScenes() const;
	
	void addWindowEventListener(IWindowEventListener* windowEventListener);
	void addKeyboardEventListener(IKeyboardEventListener* keyboardEventListener);
	void addMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener);
	void addMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener);
	void addMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener);
	void removeWindowEventListener(IWindowEventListener* windowEventListener);
	void removeKeyboardEventListener(IKeyboardEventListener* keyboardEventListener);
	void removeMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener);
	void removeMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener);
	void removeMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener);
	
	void addWindowEventListener(scripting::ScriptObjectHandle windowEventListener);
	void addKeyboardEventListener(scripting::ScriptObjectHandle keyboardEventListener);
	void addMouseMotionEventListener(scripting::ScriptObjectHandle mouseMotionEventListener);
	void addMouseButtonEventListener(scripting::ScriptObjectHandle mouseButtonEventListener);
	void addMouseWheelEventListener(scripting::ScriptObjectHandle mouseWheelEventListener);
	void removeWindowEventListener(scripting::ScriptObjectHandle windowEventListener);
	void removeKeyboardEventListener(scripting::ScriptObjectHandle keyboardEventListener);
	void removeMouseMotionEventListener(scripting::ScriptObjectHandle mouseMotionEventListener);
	void removeMouseButtonEventListener(scripting::ScriptObjectHandle mouseButtonEventListener);
	void removeMouseWheelEventListener(scripting::ScriptObjectHandle mouseWheelEventListener);
	
	void addConnectEventListener(IConnectEventListener* connectEventListener);
	void addDisconnectEventListener(IDisconnectEventListener* disconnectEventListener);
	void addMessageEventListener(IMessageEventListener* messageEventListener);
	void removeConnectEventListener(IConnectEventListener* connectEventListener);
	void removeDisconnectEventListener(IDisconnectEventListener* disconnectEventListener);
	void removeMessageEventListener(IMessageEventListener* messageEventListener);
	
	void addConnectEventListener(scripting::ScriptObjectHandle connectEventListener);
	void addDisconnectEventListener(scripting::ScriptObjectHandle disconnectEventListener);
	void addMessageEventListener(scripting::ScriptObjectHandle messageEventListener);
	void removeConnectEventListener(scripting::ScriptObjectHandle connectEventListener);
	void removeDisconnectEventListener(scripting::ScriptObjectHandle disconnectEventListener);
	void removeMessageEventListener(scripting::ScriptObjectHandle messageEventListener);
	
	// Implements the graphics::IEventListener interface
	virtual bool processEvent(const graphics::Event& event) override;

	// Implements the networking::IEventListener interface
	virtual bool processEvent(const networking::ConnectEvent& event) override;
	virtual bool processEvent(const networking::DisconnectEvent& event) override;
	virtual bool processEvent(const networking::MessageEvent& event) override;

private:
	std::unique_ptr< utilities::Properties > properties_;
	std::unique_ptr< logger::ILogger > logger_;
	std::unique_ptr<fs::IFileSystem> fileSystem_;
	
	//Graphics
	std::unique_ptr<graphics::IGraphicsEngineFactory> graphicsEngineFactory_;
	std::unique_ptr< graphics::IGraphicsEngine > graphicsEngine_;
	std::vector<std::unique_ptr< graphics::gui::IGui >> guis_;
	
	std::unique_ptr<ITerrainFactory> terrainFactory_;
	
	std::unique_ptr<IDebugRenderer> debugRenderer_;
	
	std::unique_ptr<audio::IAudioEngineFactory> audioEngineFactory_;
	std::unique_ptr< audio::IAudioEngine > audioEngine_;
		
	std::unique_ptr<networking::INetworkingEngineFactory> networkingEngineFactory_;
	std::unique_ptr< networking::INetworkingEngine > networkingEngine_;
	
	std::unique_ptr< physics::IPhysicsEngine > physicsEngine_;
	std::unique_ptr<pathfinding::IPathfindingEngineFactory> pathfindingEngineFactory_;
	std::unique_ptr< pathfinding::IPathfindingEngine > pathfindingEngine_;
	std::unique_ptr<scripting::IScriptingEngine> scriptingEngine_;
	
	std::vector<std::unique_ptr< IModule >> modules_;
	
	std::unique_ptr<ice_engine::IPluginManager> pluginManager_;
	
	std::vector<IWindowEventListener*> windowEventListeners_;
	std::vector<IKeyboardEventListener*> keyboardEventListeners_;
	std::vector<IMouseMotionEventListener*> mouseMotionEventListeners_;
	std::vector<IMouseButtonEventListener*> mouseButtonEventListeners_;
	std::vector<IMouseWheelEventListener*> mouseWheelEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptWindowEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptKeyboardEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseMotionEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseButtonEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseWheelEventListeners_;
	
	
	std::vector<IConnectEventListener*> connectEventListeners_;
	std::vector<IDisconnectEventListener*> disconnectEventListeners_;
	std::vector<IMessageEventListener*> messageEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptConnectEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptDisconnectEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMessageEventListeners_;
	
	std::vector<std::unique_ptr<Scene>> scenes_;
	
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
	void initializeAudioSubSystem();
	void initializeNetworkingSubSystem();
	void initializeInputSubSystem();
	void initializeScriptingSubSystem();
	void initializeThreadingSubSystem();
	void initializeTerrainSubSystem();
	void initializeDataStoreSubSystem();
	void initializeEntitySubSystem();
	void initializeModuleSubSystem();
	
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
