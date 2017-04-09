#ifndef IPHYSICSENGINE_H_
#define IPHYSICSENGINE_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "physics/CollisionShapeHandle.hpp"
#include "entities/Entity.hpp"
#include "IScene.hpp"

namespace hercules
{
namespace physics
{

class IPhysicsEngine
{
public:
	virtual ~IPhysicsEngine()
	{
	}
	;
	
	virtual void tick(const float32 delta) = 0;
	
	virtual CollisionShapeHandle createStaticPlane(const glm::vec3& planeNormal, const float32 planeConstant, entities::Entity entity, IScene* scene) = 0;
	virtual CollisionShapeHandle createBoxShape(const glm::vec3& dimensions, entities::Entity entity, IScene* scene) = 0;
};

}
}

#endif /* IPHYSICSENGINE_H_ */
