#ifndef PHYSICSENGINE_H_
#define PHYSICSENGINE_H_

#include <memory>
#include <vector>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include "physics/IPhysicsEngine.hpp"
#include "physics/CollisionShapeHandle.hpp"
#include "physics/bullet/BulletMotionState.hpp"
#include "DebugRenderer.hpp"

#include "handles/HandleVector.hpp"
#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace hercules
{
namespace physics
{
namespace bullet
{

struct BulletPhysicsScene
{
	std::unique_ptr<btBroadphaseInterface> broadphase;
	std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
	std::unique_ptr<btCollisionDispatcher> dispatcher;
	std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
	std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
};

enum BulletCollisionObjectType
{
	UNKNOWN = 0,
	RIGID_BODY,
	GHOST
};

struct BulletUserData
{
	BulletUserData()
	{
		
	}
	
	~BulletUserData()
	{
		
	}
	
	union
	{
		RigidBodyObjectHandle rigidBodyObjectHandle;
		GhostObjectHandle ghostObjectHandle;
	};
	BulletCollisionObjectType type;
	UserData userData;
};

struct BulletRigidBodyData
{
	std::unique_ptr<BulletMotionState> motionState;
	std::unique_ptr<btRigidBody> rigidBody;
	std::unique_ptr<BulletUserData> userData;
};

struct BulletGhostObjectData
{
	std::unique_ptr<btGhostObject> ghostObject;
	std::unique_ptr<BulletUserData> userData;
};

class PhysicsEngine : public IPhysicsEngine
{
public:
	PhysicsEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
	virtual ~PhysicsEngine();
	
	virtual void tick(const PhysicsSceneHandle& physicsSceneHandle, const float32 delta) override;
	
	virtual PhysicsSceneHandle createPhysicsScene() override;
	virtual void destroyPhysicsScene(const PhysicsSceneHandle& physicsSceneHandle) override;
	
	virtual void setGravity(const PhysicsSceneHandle& physicsSceneHandle, const glm::vec3& gravity) override;
	
	virtual void setPhysicsDebugRenderer(IPhysicsDebugRenderer* physicsDebugRenderer) override;
	
	virtual CollisionShapeHandle createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant) override;
	virtual CollisionShapeHandle createStaticBoxShape(const glm::vec3& dimensions) override;
	virtual void destroyStaticShape(const CollisionShapeHandle& collisionShapeHandle) override;
	virtual void destroyAllStaticShapes() override;
	
	virtual RigidBodyObjectHandle createDynamicRigidBodyObject(
		const PhysicsSceneHandle& physicsSceneHandle, 
		const CollisionShapeHandle& collisionShapeHandle,
		std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr,
		const UserData& userData = UserData()
	) override;
	virtual RigidBodyObjectHandle createDynamicRigidBodyObject(
		const PhysicsSceneHandle& physicsSceneHandle, 
		const CollisionShapeHandle& collisionShapeHandle,
		const float32 mass,
		const float32 friction,
		const float32 restitution,
		std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr,
		const UserData& userData = UserData()
	) override;
	virtual RigidBodyObjectHandle createDynamicRigidBodyObject(
		const PhysicsSceneHandle& physicsSceneHandle, 
		const CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const float32 mass = 1.0f,
		const float32 friction = 1.0f,
		const float32 restitution = 1.0f,
		std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr,
		const UserData& userData = UserData()
	) override;
	virtual RigidBodyObjectHandle createStaticRigidBodyObject(const PhysicsSceneHandle& physicsSceneHandle, const CollisionShapeHandle& collisionShapeHandle, const UserData& userData = UserData()) override;
	virtual RigidBodyObjectHandle createStaticRigidBodyObject(
		const PhysicsSceneHandle& physicsSceneHandle, 
		const CollisionShapeHandle& collisionShapeHandle,
		const float32 friction,
		const float32 restitution,
		const UserData& userData = UserData()
	) override;
	virtual RigidBodyObjectHandle createStaticRigidBodyObject(
		const PhysicsSceneHandle& physicsSceneHandle, 
		const CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const float32 friction = 1.0f,
		const float32 restitution = 1.0f,
		const UserData& userData = UserData()
	) override;
	virtual GhostObjectHandle createGhostObject(const PhysicsSceneHandle& physicsSceneHandle, const CollisionShapeHandle& collisionShapeHandle, const UserData& userData = UserData()) override;
	virtual GhostObjectHandle createGhostObject(
		const PhysicsSceneHandle& physicsSceneHandle, 
		const CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const UserData& userData = UserData()
	) override;
	virtual void destroyRigidBody(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle) override;
	virtual void destroyAllRigidBodies() override;
	
	virtual void setUserData(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle, const UserData& userData) override;
	virtual void setUserData(const PhysicsSceneHandle& physicsSceneHandle, const GhostObjectHandle& ghostObjectHandle, const UserData& userData) override;
	virtual UserData& getUserData(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle) const override;
	virtual UserData& getUserData(const PhysicsSceneHandle& physicsSceneHandle, const GhostObjectHandle& ghostObjectHandle) const override;
	
	virtual Raycast raycast(const PhysicsSceneHandle& physicsSceneHandle, const ray::Ray& ray) override;
	
	virtual void setMotionChangeListener(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle, std::unique_ptr<IMotionChangeListener> motionStateListener) override;
	
	virtual void rotation(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle, const glm::quat& orientation) override;
	virtual glm::quat rotation(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle) const override;
	
	virtual void position(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle, const float32 x, const float32 y, const float32 z) override;
	virtual void position(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle, const glm::vec3& position) override;
	virtual glm::vec3 position(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle) const override;
	
	virtual void position(const PhysicsSceneHandle& physicsSceneHandle, const GhostObjectHandle& ghostObjectHandle, const float32 x, const float32 y, const float32 z) override;
	virtual void position(const PhysicsSceneHandle& physicsSceneHandle, const GhostObjectHandle& ghostObjectHandle, const glm::vec3& position) override;
	virtual glm::vec3 position(const PhysicsSceneHandle& physicsSceneHandle, const GhostObjectHandle& ghostObjectHandle) const override;
	
	virtual void mass(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle, const float32 mass) override;
	virtual float32 mass(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle) const override;
	
	virtual void friction(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle, const float32 friction) override;
	virtual float32 friction(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle) const override;
	
	virtual void restitution(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle, const float32 restitution) override;
	virtual float32 restitution(const PhysicsSceneHandle& physicsSceneHandle, const RigidBodyObjectHandle& rigidBodyObjectHandle) const override;

private:
	PhysicsEngine(const PhysicsEngine& other);
	
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	
	
	std::vector<std::unique_ptr<btCollisionShape>> shapes_;
	std::vector<std::unique_ptr<BulletRigidBodyData>> rigidBodyData_;
	std::vector<std::unique_ptr<BulletGhostObjectData>> ghostObjectData_;
	//handles::HandleVector<BulletRigidBodyData, RigidBodyObjectHandle> rigidBodyData_;
	//handles::HandleVector<BulletGhostObjectData, GhostObjectHandle> ghostObjectData_;
	handles::HandleVector<BulletPhysicsScene, PhysicsSceneHandle> physicsScenes_;
	
	std::unique_ptr<DebugRenderer> debugRenderer_;
};

}
}
}

#endif /* PHYSICSENGINE_H_ */
