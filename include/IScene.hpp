#ifndef ISCENE_H_
#define ISCENE_H_

#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "entities/Entity.hpp"
#include "entities/GraphicsComponent.hpp"
#include "entities/PhysicsComponent.hpp"

#include "graphics/IGraphicsEngine.hpp"

namespace hercules
{

class IScene
{
public:
	virtual ~IScene()
	{
	}
	;
	
	virtual void tick(const float32 elapsedTime) = 0;
	
	virtual std::string getName() const = 0;
	
	virtual entities::Entity createEntity() = 0;
	
	virtual void assign(const entities::Entity& entity, const entities::GraphicsComponent& component) = 0;
	virtual void assign(const entities::Entity& entity, const entities::PhysicsComponent& component) = 0;
	
	virtual void rotate(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) = 0;
	virtual void rotate(const entities::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) = 0;
	virtual void rotation(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis) = 0;
	virtual void rotation(const entities::Entity& entity, const glm::quat& orientation) = 0;
	virtual void translate(const entities::Entity& entity, const glm::vec3& translate) = 0;
	virtual void scale(const entities::Entity& entity, const float32 scale) = 0;
	virtual void scale(const entities::Entity& entity, const glm::vec3& scale) = 0;
	virtual void scale(const entities::Entity& entity, const float32 x, const float32 y, const float32 z) = 0;
	virtual void lookAt(const entities::Entity& entity, const glm::vec3& lookAt) = 0;
	
	virtual void position(const entities::Entity& entity, const glm::vec3& position) = 0;
	virtual void position(const entities::Entity& entity, const float32 x, const float32 y, const float32 z) = 0;
};

}

#endif /* ISCENE_H_ */
