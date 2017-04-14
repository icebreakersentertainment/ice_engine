#include "logger/ILogger.hpp"

#include "physics/bullet/PhysicsEngine.hpp"
#include "physics/CollisionShapeHandle.hpp"

namespace hercules
{
namespace physics
{
namespace bullet
{

PhysicsEngine::PhysicsEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger)
{
	properties_ = properties;
	fileSystem_ = fileSystem;
	logger_ = logger;
	
	broadphase_ = std::make_unique<btDbvtBroadphase>();
	collisionConfiguration_ = std::make_unique<btDefaultCollisionConfiguration>();
	dispatcher_ = std::make_unique<btCollisionDispatcher>(collisionConfiguration_.get());
	solver_ = std::make_unique<btSequentialImpulseConstraintSolver>();
	dynamicsWorld_ = std::make_unique<btDiscreteDynamicsWorld>(dispatcher_.get(), broadphase_.get(), solver_.get(), collisionConfiguration_.get());
	
	dynamicsWorld_->setGravity(btVector3(0, -10, 0));
}

PhysicsEngine::PhysicsEngine(const PhysicsEngine& other)
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::tick(const float32 delta)
{
	dynamicsWorld_->stepSimulation(delta, 10);
}

void PhysicsEngine::setGravity(const glm::vec3& gravity)
{
	dynamicsWorld_->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

CollisionShapeHandle PhysicsEngine::createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant)
{
	auto shape = std::make_unique<btStaticPlaneShape>(btVector3(planeNormal.x, planeNormal.y, planeNormal.z), btScalar(planeConstant));
	
	shapes_.push_back(std::move(shape));
	auto index = shapes_.size() - 1;
	
	return CollisionShapeHandle(index);
}

CollisionShapeHandle PhysicsEngine::createStaticBoxShape(const glm::vec3& dimensions)
{
	auto shape = std::make_unique<btBoxShape>(btVector3(dimensions.x, dimensions.y, dimensions.z));
	
	shapes_.push_back(std::move(shape));
	auto index = shapes_.size() - 1;
	
	return CollisionShapeHandle(index);
}

void PhysicsEngine::destroyStaticShape(const CollisionShapeHandle& collisionShapeHandle)
{
	//std::vector<std::unique_ptr<btCollisionShape>> shapes_;
	//std::vector<BulletPhysicsData> physicsData_;
}

void PhysicsEngine::destroyAllStaticShapes()
{
	shapes_.clear();
}

CollisionBodyHandle PhysicsEngine::createDynamicRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	std::unique_ptr<IMotionChangeListener> motionStateListener
)
{
	return createDynamicRigidBody(collisionShapeHandle, glm::vec3(), glm::quat(), 1.0f, 1.0f, 1.0f, std::move(motionStateListener));
}

CollisionBodyHandle PhysicsEngine::createDynamicRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	const float32 mass,
	const float32 friction,
	const float32 restitution,
	std::unique_ptr<IMotionChangeListener> motionStateListener
)
{
	return createDynamicRigidBody(collisionShapeHandle, glm::vec3(), glm::quat(), mass, friction, restitution, std::move(motionStateListener));
}

CollisionBodyHandle PhysicsEngine::createDynamicRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 mass,
	const float32 friction,
	const float32 restitution,
	std::unique_ptr<IMotionChangeListener> motionStateListener
)
{
	auto& shape = shapes_[collisionShapeHandle.getId()];
	
	btTransform transform;
	transform.setRotation( btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w) );
	transform.setOrigin( btVector3(position.x, position.y, position.z) );
	
	btScalar bulletMass(mass);
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (bulletMass != btScalar(0.0f))
	{
		shape->calculateLocalInertia(bulletMass, localInertia);
	}
	
	BulletPhysicsData physicsData;
	
	physicsData.motionState = std::make_unique<BulletMotionState>(transform, std::move(motionStateListener));
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(bulletMass, physicsData.motionState.get(), shape.get(), localInertia);
	constructionInfo.m_friction = friction;
	constructionInfo.m_restitution = restitution;
	physicsData.rigidBody = std::make_unique<btRigidBody>(constructionInfo);
	
	dynamicsWorld_->addRigidBody(physicsData.rigidBody.get());
	
	physicsData_.push_back(std::move(physicsData));
	auto index = physicsData_.size() - 1;
	
	return CollisionBodyHandle(index);
}

CollisionBodyHandle PhysicsEngine::createStaticRigidBody(const CollisionShapeHandle& collisionShapeHandle)
{
	return createStaticRigidBody(collisionShapeHandle, glm::vec3(), glm::quat(), 1.0f, 1.0f);
}

CollisionBodyHandle PhysicsEngine::createStaticRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	const float32 friction,
	const float32 restitution
)
{
	return createStaticRigidBody(collisionShapeHandle, glm::vec3(), glm::quat(), friction, restitution);
}

CollisionBodyHandle PhysicsEngine::createStaticRigidBody(
	const CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 friction,
	const float32 restitution
)
{
	auto& shape = shapes_[collisionShapeHandle.getId()];
	
	btTransform transform;
	transform.setRotation( btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w) );
	transform.setOrigin( btVector3(position.x, position.y, position.z) );
	
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	
	BulletPhysicsData physicsData;
	
	physicsData.motionState = std::make_unique<BulletMotionState>(transform, nullptr);
	btRigidBody::btRigidBodyConstructionInfo constructionInfo(btScalar(0.0f), physicsData.motionState.get(), shape.get(), localInertia);
	constructionInfo.m_friction = friction;
	constructionInfo.m_restitution = restitution;
	physicsData.rigidBody = std::make_unique<btRigidBody>(constructionInfo);
	
	dynamicsWorld_->addRigidBody(physicsData.rigidBody.get());
	
	physicsData_.push_back(std::move(physicsData));
	auto index = physicsData_.size() - 1;
	
	return CollisionBodyHandle(index);
}

void PhysicsEngine::destroyRigidBody(const CollisionBodyHandle& collisionBodyHandle)
{
	
}

void PhysicsEngine::destroyAllRigidBodies()
{
	physicsData_.clear();
}

void PhysicsEngine::setMotionChangeListener(const CollisionBodyHandle& collisionBodyHandle, std::unique_ptr<IMotionChangeListener> motionStateListener)
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	physicsData.motionState->setMotionChangeListener(std::move(motionStateListener));
}

void PhysicsEngine::rotation(const CollisionBodyHandle& collisionBodyHandle, const glm::quat& orientation)
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	btTransform transform = physicsData.motionState->getWorldTransform();
	
	const btQuaternion quat = btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
	transform.setRotation(quat);
	
	physicsData.rigidBody->setWorldTransform(transform);
}

glm::quat PhysicsEngine::rotation(const CollisionBodyHandle& collisionBodyHandle) const
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	const btQuaternion rotation = physicsData.motionState->getWorldTransform().getRotation();
	
	return glm::quat(rotation.w(), rotation.x(), rotation.y(), rotation.z());
}

void PhysicsEngine::position(const CollisionBodyHandle& collisionBodyHandle, const float32 x, const float32 y, const float32 z)
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	btTransform transform = physicsData.motionState->getWorldTransform();
	
	const btVector3 pos = btVector3(x, y, z);
	transform.setOrigin(pos);
	
	physicsData.rigidBody->setWorldTransform(transform);
}

void PhysicsEngine::position(const CollisionBodyHandle& collisionBodyHandle, const glm::vec3& position)
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];

	btTransform transform = physicsData.motionState->getWorldTransform();
	
	const btVector3 pos = btVector3(position.x, position.y, position.z);
	transform.setOrigin(pos);
	
	physicsData.rigidBody->setWorldTransform(transform);
}

glm::vec3 PhysicsEngine::position(const CollisionBodyHandle& collisionBodyHandle) const
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	const btVector3 position = physicsData.motionState->getWorldTransform().getOrigin();
	
	return glm::vec3(position.x(), position.y(), position.z());
}

void PhysicsEngine::mass(const CollisionBodyHandle& collisionBodyHandle, const float32 mass)
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	auto shape = physicsData.rigidBody->getCollisionShape();
	
	btScalar bulletMass(mass);
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	if (bulletMass != btScalar(0.0f))
	{
		shape->calculateLocalInertia(bulletMass, localInertia);
	}
	
	physicsData.rigidBody->setMassProps(bulletMass, localInertia);
}

float32 PhysicsEngine::mass(const CollisionBodyHandle& collisionBodyHandle) const
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	const auto inverseMass = physicsData.rigidBody->getInvMass();
	
	return pow(inverseMass, -1.0f);
}

void PhysicsEngine::friction(const CollisionBodyHandle& collisionBodyHandle, const float32 friction)
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	physicsData.rigidBody->setFriction(btScalar(friction));
}

float32 PhysicsEngine::friction(const CollisionBodyHandle& collisionBodyHandle) const
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	const btScalar friction = physicsData.rigidBody->getFriction();
	
	return friction;
}

void PhysicsEngine::restitution(const CollisionBodyHandle& collisionBodyHandle, const float32 restitution)
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	physicsData.rigidBody->setRestitution(btScalar(restitution));
}

float32 PhysicsEngine::restitution(const CollisionBodyHandle& collisionBodyHandle) const
{
	auto& physicsData = physicsData_[collisionBodyHandle.getId()];
	
	const btScalar restitution = physicsData.rigidBody->getRestitution();
	
	return restitution;
}

}
}
}
