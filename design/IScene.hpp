virtual void tick(const float32 elapsedTime) = 0;

virtual physics::CollisionShapeHandle createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant) = 0;
virtual physics::CollisionShapeHandle createStaticBoxShape(const glm::vec3& dimensions) = 0;
virtual void destroyStaticShape(const physics::CollisionShapeHandle& collisionShapeHandle) = 0;
virtual void destroyAllStaticShapes() = 0;

virtual physics::CollisionBodyHandle createDynamicRigidBody(const physics::CollisionShapeHandle& collisionShapeHandle) = 0;
virtual physics::CollisionBodyHandle createDynamicRigidBody(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const float32 mass,
	const float32 friction,
	const float32 restitution
) = 0;
virtual physics::CollisionBodyHandle createDynamicRigidBody(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 mass = 1.0f,
	const float32 friction = 1.0f,
	const float32 restitution = 1.0f
) = 0;
virtual physics::CollisionBodyHandle createStaticRigidBody(const physics::CollisionShapeHandle& collisionShapeHandle) = 0;
virtual physics::CollisionBodyHandle createStaticRigidBody(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const float32 friction,
	const float32 restitution
) = 0;
virtual physics::CollisionBodyHandle createStaticRigidBody(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 friction = 1.0f,
	const float32 restitution = 1.0f
) = 0;

virtual graphics::RenderableHandle createRenderable(const ModelHandle& modelHandle, const std::string& name = std::string()) = 0;

virtual std::string getName() const = 0;

virtual entities::Entity createEntity() = 0;
virtual entities::Entity destroyEntity(const entities::Entity& entity) = 0;
virtual uint32 getNumEntities() const = 0;

virtual void assign(const entities::Entity& entity, const entities::GraphicsComponent& component) = 0;
virtual void assign(const entities::Entity& entity, const entities::PhysicsComponent& component) = 0;
virtual void assign(const entities::Entity& entity, const entities::PositionOrientationComponent& component) = 0;

virtual void rotate(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) = 0;
virtual void rotate(const entities::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) = 0;
virtual void rotation(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis) = 0;
virtual void rotation(const entities::Entity& entity, const glm::quat& orientation) = 0;
virtual glm::quat rotation(const entities::Entity& entity) const = 0;
virtual void translate(const entities::Entity& entity, const glm::vec3& translate) = 0;
virtual void scale(const entities::Entity& entity, const float32 scale) = 0;
virtual void scale(const entities::Entity& entity, const glm::vec3& scale) = 0;
virtual void scale(const entities::Entity& entity, const float32 x, const float32 y, const float32 z) = 0;
virtual glm::vec3 scale(const entities::Entity& entity) const = 0;
virtual void lookAt(const entities::Entity& entity, const glm::vec3& lookAt) = 0;

virtual void position(const entities::Entity& entity, const glm::vec3& position) = 0;
virtual void position(const entities::Entity& entity, const float32 x, const float32 y, const float32 z) = 0;
virtual glm::vec3 position(const entities::Entity& entity) const = 0;
