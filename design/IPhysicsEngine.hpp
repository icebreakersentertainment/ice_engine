class IMotionChangeListener
{
};
class CollisionShapeHandle
{
};
class CollisionBodyHandle
{
};
//class CollisionShape
//{
//};
class PhysicsSceneHandle
{
};

virtual void tick(const PhysicsSceneHandle& physicsSceneHandle, const float32 delta) = 0;
	
virtual void setGravity(const PhysicsSceneHandle& physicsSceneHandle, const glm::vec3& gravity) = 0;



/* SHAPE STUFF */
virtual CollisionShapeHandle createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant) = 0;
virtual CollisionShapeHandle createStaticBoxShape(const glm::vec3& dimensions) = 0;
virtual void destroyStaticShape(const CollisionShapeHandle& collisionShapeHandle) = 0;
virtual void destroyAllStaticShapes() = 0;



/* RIGID BODY STUFF */
virtual CollisionBodyHandle createDynamicRigidBody(
	const PhysicsSceneHandle& physicsSceneHandle, 
	const CollisionShapeHandle& collisionShapeHandle,
	std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr
) = 0;
virtual CollisionBodyHandle createDynamicRigidBody(
	const PhysicsSceneHandle& physicsSceneHandle, 
	const CollisionShapeHandle& collisionShapeHandle,
	const float32 mass,
	const float32 friction,
	const float32 restitution,
	std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr
) = 0;
virtual CollisionBodyHandle createDynamicRigidBody(
	const PhysicsSceneHandle& physicsSceneHandle,
	const CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 mass = 1.0f,
	const float32 friction = 1.0f,
	const float32 restitution = 1.0f,
	std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr
) = 0;
virtual CollisionBodyHandle createStaticRigidBody(const PhysicsSceneHandle& physicsSceneHandle, const CollisionShapeHandle& collisionShapeHandle) = 0;
virtual CollisionBodyHandle createStaticRigidBody(
	const PhysicsSceneHandle& physicsSceneHandle,
	const CollisionShapeHandle& collisionShapeHandle,
	const float32 friction,
	const float32 restitution
) = 0;
virtual CollisionBodyHandle createStaticRigidBody(
	const PhysicsSceneHandle& physicsSceneHandle,
	const CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 friction = 1.0f,
	const float32 restitution = 1.0f
) = 0;
virtual void destroyRigidBody(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle) = 0;
virtual void destroyAllRigidBodies(const PhysicsSceneHandle& physicsSceneHandle) = 0;



/* CHANGE LISTENER */
virtual void setMotionChangeListener(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle, std::unique_ptr<IMotionChangeListener> motionStateListener) = 0;



/* BODY MOVEMENT AND SETTING/GETTING PROPERTIES */
virtual void rotation(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle, const glm::quat& orientation) = 0;
virtual glm::quat rotation(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle) const = 0;
virtual void position(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void position(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle, const glm::vec3& position) = 0;
virtual glm::vec3 position(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle) const = 0;
virtual void mass(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle, const float32 mass) = 0;
/**
 * Note: This value must be calculated, so it may not be exactly what you set originally.
 */
virtual float32 mass(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle) const = 0;
virtual void friction(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle, const float32 friction) = 0;
virtual float32 friction(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle) const = 0;
virtual void restitution(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle, const float32 restitution) = 0;
virtual float32 restitution(const PhysicsSceneHandle& physicsSceneHandle, const CollisionBodyHandle& collisionBodyHandle) const = 0;
