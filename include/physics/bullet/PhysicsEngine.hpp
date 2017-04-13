#ifndef PHYSICSENGINE_H_
#define PHYSICSENGINE_H_

#include <memory>
#include <vector>

#include <bullet/btBulletDynamicsCommon.h>

#include "physics/IPhysicsEngine.hpp"
#include "physics/CollisionShapeHandle.hpp"
#include "physics/bullet/BulletMotionState.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace hercules
{
namespace physics
{
namespace bullet
{

struct BulletPhysicsData
{
	btCollisionShape* shape;
	BulletMotionState* motionState;
	btRigidBody* rigidBody;
};

class PhysicsEngine : public IPhysicsEngine
{
public:
	PhysicsEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
	virtual ~PhysicsEngine();
	
	virtual void tick(const float32 delta) override;
	
	virtual void setGravity(const glm::vec3& gravity) override;
	
	virtual CollisionShapeHandle createStaticPlane(const glm::vec3& planeNormal, const float32 planeConstant, std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr) override;
	virtual CollisionShapeHandle createBoxShape(const glm::vec3& dimensions, std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr) override;
	
	virtual void rotation(const CollisionShapeHandle& collisionShapeHandle, const glm::quat& orientation) override;
	
	//virtual void scale(const CollisionShapeHandle& collisionShapeHandle, const float32 x, const float32 y, const float32 z) override;
	//virtual void scale(const CollisionShapeHandle& collisionShapeHandle, const glm::vec3& scale) override;
	//virtual void scale(const CollisionShapeHandle& collisionShapeHandle, const float32 scale) override;
	
	virtual void position(const CollisionShapeHandle& collisionShapeHandle, const float32 x, const float32 y, const float32 z) override;
	virtual void position(const CollisionShapeHandle& collisionShapeHandle, const glm::vec3& position) override;

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
	
	std::vector<BulletPhysicsData> bulletPhysicsData_;
	std::vector<BulletMotionState*> bulletMotionStates_;
};

}
}
}

#endif /* PHYSICSENGINE_H_ */
