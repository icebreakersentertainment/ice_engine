#ifndef PHYSICSENGINE_H_
#define PHYSICSENGINE_H_

#include <memory>
#include <vector>

#include <bullet/btBulletDynamicsCommon.h>

#include "physics/IPhysicsEngine.hpp"

namespace physics
{
namespace bullet
{

class PhysicsEngine : public IPhysicsEngine
{
public:
	PhysicsEngine();
	virtual ~PhysicsEngine();
	
	virtual void tick(glm::detail::float32 delta);
	
	virtual ICollisionShape* createStaticPlane(const glm::vec3& planeNormal, glm::detail::float32 planeConstant);

private:
	PhysicsEngine(const PhysicsEngine& other);
	
	std::unique_ptr<btBroadphaseInterface> broadphase_;
	std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration_;
	std::unique_ptr<btCollisionDispatcher> dispatcher_;
	std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;
	std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld_;
	
	std::vector< std::unique_ptr<ICollisionShape> > shapes_;
};

}
}

#endif /* PHYSICSENGINE_H_ */