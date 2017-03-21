#ifndef PHYSICSENGINE_H_
#define PHYSICSENGINE_H_

#include <memory>
#include <vector>

#include <bullet/btBulletDynamicsCommon.h>

#include "physics/IPhysicsEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace hercules
{
namespace physics
{
namespace bullet
{

class PhysicsEngine : public IPhysicsEngine
{
public:
	PhysicsEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
	virtual ~PhysicsEngine();
	
	virtual void tick(float32 delta) override;
	
	virtual ICollisionShape* createStaticPlane(const glm::vec3& planeNormal, float32 planeConstant) override;

private:
	PhysicsEngine(const PhysicsEngine& other);
	
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	std::unique_ptr<btBroadphaseInterface> broadphase_;
	std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration_;
	std::unique_ptr<btCollisionDispatcher> dispatcher_;
	std::unique_ptr<btSequentialImpulseConstraintSolver> solver_;
	std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld_;
	
	std::vector< std::unique_ptr<ICollisionShape> > shapes_;
};

}
}
}

#endif /* PHYSICSENGINE_H_ */
