#ifndef IPHYSICSENGINE_H_
#define IPHYSICSENGINE_H_

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "physics/CollisionShapeHandle.hpp"
#include "physics/IMotionStateListener.hpp"

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
	
	virtual CollisionShapeHandle createStaticPlane(const glm::vec3& planeNormal, const float32 planeConstant, std::unique_ptr<IMotionStateListener> motionStateListener = nullptr) = 0;
	virtual CollisionShapeHandle createBoxShape(const glm::vec3& dimensions, std::unique_ptr<IMotionStateListener> motionStateListener = nullptr) = 0;
};

}
}

#endif /* IPHYSICSENGINE_H_ */
