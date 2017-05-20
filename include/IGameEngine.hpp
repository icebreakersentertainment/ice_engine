#ifndef IGAMEENGINE_H_
#define IGAMEENGINE_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "ModelHandle.hpp"
#include "IScene.hpp"

#include "graphics/IGraphicsEngine.hpp"

#include "graphics/model/Model.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "scripting/IScriptingEngine.hpp"

namespace hercules
{

// game states
enum GameState
{
	GAME_STATE_UNKNOWN = 0,
	GAME_STATE_STARTUP,
	GAME_STATE_MAIN_MENU,
	GAME_STATE_START_NEW_GAME,
	GAME_STATE_LOAD_NEW_GAME,
	GAME_STATE_IN_GAME
};

class IGameEngine
{
public:
	virtual ~IGameEngine()
	{
	}
	;
	
	virtual void run() = 0;
	
	virtual GameState getState() = 0;
	
	virtual void setIGameInstance(asIScriptObject* obj) = 0;
	virtual void setBootstrapScript(const std::string& filename) = 0;
	
	virtual graphics::IGraphicsEngine* getGraphicsEngine() const = 0;
	virtual physics::IPhysicsEngine* getPhysicsEngine() const = 0;
	
	//virtual AudioSample* loadAudioSample(const std::string& name, const std::string& filename) = 0;
	virtual utilities::Image* loadImage(const std::string& name, const std::string& filename) = 0;
	virtual graphics::model::Model* loadModel(const std::string& name, const std::string& filename) = 0;
	virtual graphics::model::Model* importModel(const std::string& name, const std::string& filename) = 0;
	
	//virtual void unloadAudioSample(const std::string& name) = 0;
	virtual void unloadImage(const std::string& name) = 0;
	virtual void unloadModel(const std::string& name) = 0;
	
	//virtual AudioSample* getAudioSample(const std::string& name) const = 0;
	virtual utilities::Image* getImage(const std::string& name) const = 0;
	virtual graphics::model::Model* getModel(const std::string& name) const = 0;
	
	virtual ModelHandle loadStaticModel(const graphics::model::Model* model) = 0;
	virtual graphics::RenderableHandle createRenderable(const ModelHandle& modelHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name = std::string()) = 0;
	
	virtual graphics::ShaderHandle createVertexShader(const std::string& name, const std::string& filename) = 0;
	virtual graphics::ShaderHandle createVertexShaderFromSource(const std::string& name, const std::string& data) = 0;
	virtual graphics::ShaderHandle createFragmentShader(const std::string& name, const std::string& filename) = 0;
	virtual graphics::ShaderHandle createFragmentShaderFromSource(const std::string& name, const std::string& data) = 0;
	virtual graphics::ShaderHandle getShader(const std::string& name) const = 0;
	virtual void destroyShader(const std::string& name) = 0;
	virtual void destroyShader(const graphics::ShaderHandle& shaderHandle) = 0;
	
	virtual graphics::ShaderProgramHandle createShaderProgram(const std::string& name, const graphics::ShaderHandle& vertexShaderHandle, const graphics::ShaderHandle& fragmentShaderHandle) = 0;
	virtual graphics::ShaderProgramHandle getShaderProgram(const std::string& name) const = 0;
	virtual void destroyShaderProgram(const std::string& name) = 0;
	virtual void destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle) = 0;
	
	virtual IScene* createScene(const std::string& name) = 0;
	virtual void destroyScene(const std::string& name) = 0;
	virtual void destroyScene(IScene* scene) = 0;
	virtual IScene* getScene(const std::string& name) const = 0;
};

}

#endif /* IGAMEENGINE_H_ */
