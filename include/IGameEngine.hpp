#ifndef IGAMEENGINE_H_
#define IGAMEENGINE_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "graphics/IGraphicsEngine.hpp"

#include "entities/Entity.hpp"
#include "entities/GraphicsComponent.hpp"

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

	virtual entities::Entity createEntity() = 0;
	
	virtual void assign(const entities::Entity entity, const entities::GraphicsComponent& component) = 0;
	
	virtual void rotate(const entities::Entity entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) = 0;
	virtual void translate(const entities::Entity entity, const glm::vec3& translate) = 0;
	virtual void setScale(const entities::Entity entity, const glm::vec3& scale) = 0;
	virtual void setScale(const entities::Entity entity, const float32 x, const float32 y, const float32 z) = 0;
	virtual void lookAt(const entities::Entity entity, const glm::vec3& lookAt) = 0;
	
	virtual void setPosition(const entities::Entity entity, const glm::vec3& position) = 0;
	virtual void setPosition(const entities::Entity entity, const float32 x, const float32 y, const float32 z) = 0;
	
	virtual void setBootstrapScript(const std::string& className, const std::string& filename) = 0;
};

}

#endif /* IGAMEENGINE_H_ */
