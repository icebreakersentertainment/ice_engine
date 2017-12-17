#ifndef IGAMEENGINE_H_
#define IGAMEENGINE_H_

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
#include "IScene.hpp"
#include "IKeyboardEventListener.hpp"
#include "IMouseMotionEventListener.hpp"
#include "IMouseButtonEventListener.hpp"
#include "IMouseWheelEventListener.hpp"

#include "graphics/IGraphicsEngine.hpp"
#include "graphics/gui/IGui.hpp"

#include "graphics/model/Model.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "pathfinding/IPathfindingEngine.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "image/Image.hpp"

namespace ice_engine
{

class IGameEngine
{
public:
	virtual ~IGameEngine()
	{
	}
	;
	
	virtual void run() = 0;
	
	virtual const EngineStatistics& getEngineStatistics() const = 0;
	
	virtual void setIGameInstance(scripting::ScriptObjectHandle scriptObjectHandle) = 0;
	virtual void setBootstrapScript(const std::string& filename) = 0;
	
	virtual graphics::IGraphicsEngine* getGraphicsEngine() const = 0;
	virtual physics::IPhysicsEngine* getPhysicsEngine() const = 0;
	virtual IDebugRenderer* getDebugRenderer() const = 0;
	virtual pathfinding::IPathfindingEngine* getPathfindingEngine() const = 0;
	virtual IThreadPool* getBackgroundThreadPool() const = 0;
	virtual IThreadPool* getForegroundThreadPool() const = 0;
	virtual IOpenGlLoader* getOpenGlLoader() const = 0;
	
	virtual graphics::gui::IGui* createGui(const std::string& name) = 0;
	virtual void destroyGui(const graphics::gui::IGui* gui) = 0;
	
	virtual void setCallback(graphics::gui::IButton* button, scripting::ScriptFunctionHandle scriptFunctionHandle) = 0;
	
	//virtual AudioSample* loadAudioSample(const std::string& name, const std::string& filename) = 0;
	virtual image::Image* loadImage(const std::string& name, const std::string& filename) = 0;
	virtual std::shared_future<image::Image*> loadImageAsync(const std::string& name, const std::string& filename) = 0;
	virtual graphics::model::Model* loadModel(const std::string& name, const std::string& filename) = 0;
	virtual std::shared_future<graphics::model::Model*> loadModelAsync(const std::string& name, const std::string& filename) = 0;
	virtual graphics::model::Model* importModel(const std::string& name, const std::string& filename) = 0;
	virtual std::shared_future<graphics::model::Model*> importModelAsync(const std::string& name, const std::string& filename) = 0;
	
	//virtual void unloadAudioSample(const std::string& name) = 0;
	virtual void unloadImage(const std::string& name) = 0;
	virtual void unloadModel(const std::string& name) = 0;
	
	//virtual AudioSample* getAudioSample(const std::string& name) const = 0;
	virtual image::Image* getImage(const std::string& name) const = 0;
	virtual graphics::model::Model* getModel(const std::string& name) const = 0;
	
	virtual ModelHandle loadStaticModel(const graphics::model::Model* model) = 0;
	virtual std::shared_future<ModelHandle> loadStaticModelAsync(const graphics::model::Model* model) = 0;
	virtual graphics::RenderableHandle createRenderable(const graphics::RenderSceneHandle& renderSceneHandle, const ModelHandle& modelHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name = std::string()) = 0;
	virtual graphics::RenderableHandle createRenderable(const graphics::RenderSceneHandle& renderSceneHandle, const graphics::MeshHandle& meshHandle, const graphics::TextureHandle& textureHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name = std::string()) = 0;
	
	virtual graphics::ShaderHandle createVertexShader(const std::string& name, const std::string& filename) = 0;
	virtual std::shared_future<graphics::ShaderHandle> createVertexShaderAsync(const std::string& name, const std::string& filename) = 0;
	virtual graphics::ShaderHandle createVertexShaderFromSource(const std::string& name, const std::string& data) = 0;
	virtual std::shared_future<graphics::ShaderHandle> createVertexShaderFromSourceAsync(const std::string& name, const std::string& data) = 0;
	virtual graphics::ShaderHandle createFragmentShader(const std::string& name, const std::string& filename) = 0;
	virtual std::shared_future<graphics::ShaderHandle> createFragmentShaderAsync(const std::string& name, const std::string& filename) = 0;
	virtual graphics::ShaderHandle createFragmentShaderFromSource(const std::string& name, const std::string& data) = 0;
	virtual std::shared_future<graphics::ShaderHandle> createFragmentShaderFromSourceAsync(const std::string& name, const std::string& data) = 0;
	virtual graphics::ShaderHandle getShader(const std::string& name) const = 0;
	virtual void destroyShader(const std::string& name) = 0;
	virtual void destroyShader(const graphics::ShaderHandle& shaderHandle) = 0;
	
	virtual graphics::ShaderProgramHandle createShaderProgram(const std::string& name, const graphics::ShaderHandle& vertexShaderHandle, const graphics::ShaderHandle& fragmentShaderHandle) = 0;
	virtual std::shared_future<graphics::ShaderProgramHandle> createShaderProgramAsync(const std::string& name, const graphics::ShaderHandle& vertexShaderHandle, const graphics::ShaderHandle& fragmentShaderHandle) = 0;
	virtual graphics::ShaderProgramHandle getShaderProgram(const std::string& name) const = 0;
	virtual void destroyShaderProgram(const std::string& name) = 0;
	virtual void destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle) = 0;
	
	virtual IScene* createScene(const std::string& name) = 0;
	virtual void destroyScene(const std::string& name) = 0;
	virtual void destroyScene(IScene* scene) = 0;
	virtual IScene* getScene(const std::string& name) const = 0;
	virtual std::vector<IScene*> getAllScenes() const = 0;
	
	virtual void addKeyboardEventListener(IKeyboardEventListener* keyboardEventListener) = 0;
	virtual void addMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener) = 0;
	virtual void addMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener) = 0;
	virtual void addMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener) = 0;
	virtual void removeKeyboardEventListener(IKeyboardEventListener* keyboardEventListener) = 0;
	virtual void removeMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener) = 0;
	virtual void removeMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener) = 0;
	virtual void removeMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener) = 0;
};

}

#endif /* IGAMEENGINE_H_ */
