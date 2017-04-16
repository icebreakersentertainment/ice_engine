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
	
	virtual graphics::model::Model importModel(const std::string& filename) const = 0;
	virtual ModelHandle loadStaticModel(const graphics::model::Model& model) = 0;
	virtual graphics::RenderableHandle createRenderable(const ModelHandle& modelHandle, const std::string& name = std::string()) = 0;
	
	virtual IScene* createScene(const std::string& name) = 0;
	virtual IScene* getScene(const std::string& name) const = 0;
};

}

#endif /* IGAMEENGINE_H_ */
