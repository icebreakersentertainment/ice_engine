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

virtual CollisionShapeHandle createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant) = 0;
virtual CollisionShapeHandle createStaticBoxShape(const glm::vec3& dimensions) = 0;
virtual void destroyStaticShape(const CollisionShapeHandle& collisionShapeHandle) = 0;
virtual void destroyAllStaticShapes() = 0;

virtual CollisionBodyHandle createDynamicRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr
) = 0;
virtual CollisionBodyHandle createDynamicRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	const float32 mass,
	const float32 friction,
	const float32 restitution,
	std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr
) = 0;
virtual CollisionBodyHandle createDynamicRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 mass = 1.0f,
	const float32 friction = 1.0f,
	const float32 restitution = 1.0f,
	std::unique_ptr<IMotionChangeListener> motionStateListener = nullptr
) = 0;
virtual CollisionBodyHandle createStaticRigidBody(const CollisionShapeHandle& collisionShapeHandle) = 0;
virtual CollisionBodyHandle createStaticRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	const float32 mass,
	const float32 friction,
	const float32 restitution
) = 0;
virtual CollisionBodyHandle createStaticRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 mass = 1.0f,
	const float32 friction = 1.0f,
	const float32 restitution = 1.0f
) = 0;
virtual void setMotionStateListener(const CollisionBodyHandle& collisionBodyHandle, std::unique_ptr<IMotionChangeListener> motionStateListener) = 0;
virtual void destroyRigidBody(const CollisionBodyHandle& collisionBodyHandle) = 0;
virtual void destroyAllRigidBodies() = 0;

virtual void rotation(const CollisionBodyHandle& collisionBodyHandle, const glm::quat& orientation) = 0;
virtual glm::quat rotation(const CollisionBodyHandle& collisionBodyHandle) const = 0;
virtual void position(const CollisionBodyHandle& collisionBodyHandle, const float32 x, const float32 y, const float32 z) = 0;
virtual void position(const CollisionBodyHandle& collisionBodyHandle, const glm::vec3& position) = 0;
virtual glm::vec3 position(const CollisionBodyHandle& collisionBodyHandle) const = 0;
virtual void mass(const CollisionBodyHandle& collisionBodyHandle, const float32 mass) = 0;
virtual float32 mass(const CollisionBodyHandle& collisionBodyHandle) const = 0;
virtual void friction(const CollisionBodyHandle& collisionBodyHandle, const float32 friction) = 0;
virtual float32 friction(const CollisionBodyHandle& collisionBodyHandle) const = 0;
virtual void restitution(const CollisionBodyHandle& collisionBodyHandle, const float32 restitution) = 0;
virtual float32 restitution(const CollisionBodyHandle& collisionBodyHandle) const = 0;
