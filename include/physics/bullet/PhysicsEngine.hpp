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
	std::unique_ptr<BulletMotionState> motionState;
	std::unique_ptr<btRigidBody> rigidBody;
};

class PhysicsEngine : public IPhysicsEngine
{
public:
	PhysicsEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
	virtual ~PhysicsEngine();
	
	virtual void tick(const float32 delta) override;
	
	virtual void setGravity(const glm::vec3& gravity) override;
	
	virtual CollisionShapeHandle createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant) override;
	virtual CollisionShapeHandle createStaticBoxShape(const glm::vec3& dimensions) override;
	virtual void destroyStaticShape(const CollisionShapeHandle& collisionShapeHandle) override;
	virtual void destroyAllStaticShapes() override;
	
	virtual CollisionBodyHandle createDynamicRigidBody(
		const CollisionShapeHandle& collisionShapeHandle,
		std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr
	) override;
	virtual CollisionBodyHandle createDynamicRigidBody(
		const CollisionShapeHandle& collisionShapeHandle,
		const float32 mass,
		const float32 friction,
		const float32 restitution,
		std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr
	) override;
	virtual CollisionBodyHandle createDynamicRigidBody(
		const CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const float32 mass = 1.0f,
		const float32 friction = 1.0f,
		const float32 restitution = 1.0f,
		std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr
	) override;
	virtual CollisionBodyHandle createStaticRigidBody(const CollisionShapeHandle& collisionShapeHandle) override;
	virtual CollisionBodyHandle createStaticRigidBody(
		const CollisionShapeHandle& collisionShapeHandle,
		const float32 friction,
		const float32 restitution
	) override;
	virtual CollisionBodyHandle createStaticRigidBody(
		const CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const float32 friction = 1.0f,
		const float32 restitution = 1.0f
	) override;
	virtual void destroyRigidBody(const CollisionBodyHandle& collisionBodyHandle) override;
	virtual void destroyAllRigidBodies() override;
	
	virtual void rotation(const CollisionBodyHandle& collisionBodyHandle, const glm::quat& orientation) override;
	virtual glm::quat rotation(const CollisionBodyHandle& collisionBodyHandle) const override;
	virtual void position(const CollisionBodyHandle& collisionBodyHandle, const float32 x, const float32 y, const float32 z) override;
	virtual void position(const CollisionBodyHandle& collisionBodyHandle, const glm::vec3& position) override;
	virtual glm::vec3 position(const CollisionBodyHandle& collisionBodyHandle) const override;
	virtual void mass(const CollisionBodyHandle& collisionBodyHandle, const float32 mass) override;
	virtual float32 mass(const CollisionBodyHandle& collisionBodyHandle) const override;
	virtual void friction(const CollisionBodyHandle& collisionBodyHandle, const float32 friction) override;
	virtual float32 friction(const CollisionBodyHandle& collisionBodyHandle) const override;
	virtual void restitution(const CollisionBodyHandle& collisionBodyHandle, const float32 restitution) override;
	virtual float32 restitution(const CollisionBodyHandle& collisionBodyHandle) const override;

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
	
	std::vector<std::unique_ptr<btCollisionShape>> shapes_;
	std::vector<BulletPhysicsData> physicsData_;
};

}
}
}

#endif /* PHYSICSENGINE_H_ */
