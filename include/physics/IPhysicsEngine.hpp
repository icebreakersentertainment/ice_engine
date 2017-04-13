#ifndef IPHYSICSENGINE_H_
#define IPHYSICSENGINE_H_

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "physics/CollisionShapeHandle.hpp"
#include "physics/IMotionChangeListener.hpp"

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
	
	virtual void setGravity(const glm::vec3& gravity) = 0;
	
	virtual CollisionShapeHandle createStaticPlane(const glm::vec3& planeNormal, const float32 planeConstant, std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr) = 0;
	virtual CollisionShapeHandle createBoxShape(const glm::vec3& dimensions, std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr) = 0;
	
	virtual void rotation(const CollisionShapeHandle& collisionShapeHandle, const glm::quat& orientation) = 0;
	
	//virtual void scale(const CollisionShapeHandle& collisionShapeHandle, const float32 x, const float32 y, const float32 z) = 0;
	//virtual void scale(const CollisionShapeHandle& collisionShapeHandle, const glm::vec3& scale) = 0;
	//virtual void scale(const CollisionShapeHandle& collisionShapeHandle, const float32 scale) = 0;
	
	virtual void position(const CollisionShapeHandle& collisionShapeHandle, const float32 x, const float32 y, const float32 z) = 0;
	virtual void position(const CollisionShapeHandle& collisionShapeHandle, const glm::vec3& position) = 0;
};

}
}

#endif /* IPHYSICSENGINE_H_ */
