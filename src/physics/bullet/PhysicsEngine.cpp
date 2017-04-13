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

CollisionShapeHandle PhysicsEngine::createStaticPlane(const glm::vec3& planeNormal, const float32 planeConstant, std::unique_ptr<IMotionChangeListener> motionStateListener)
{
	BulletPhysicsData bulletPhysicsData;
	bulletPhysicsData.shape = new btStaticPlaneShape(btVector3(planeNormal.x, planeNormal.y, planeNormal.z), planeConstant);
	
	btTransform groundShapeTransform;
	groundShapeTransform.setIdentity();
	groundShapeTransform.setOrigin(btVector3());
	
	bulletPhysicsData.motionState = new BulletMotionState(groundShapeTransform, std::move(motionStateListener));
	btRigidBody::btRigidBodyConstructionInfo rbGroundInfo(0, bulletPhysicsData.motionState, bulletPhysicsData.shape);
	rbGroundInfo.m_friction = 1.0f;
	rbGroundInfo.m_restitution = 1.0f;
	bulletPhysicsData.rigidBody = new btRigidBody(rbGroundInfo);
	
	dynamicsWorld_->addRigidBody(bulletPhysicsData.rigidBody);
	
	bulletPhysicsData_.push_back(bulletPhysicsData);
	auto index = bulletPhysicsData_.size() - 1;
	
	return CollisionShapeHandle(index);
}

CollisionShapeHandle PhysicsEngine::createBoxShape(const glm::vec3& dimensions, std::unique_ptr<IMotionChangeListener> motionStateListener)
{
	BulletPhysicsData bulletPhysicsData;
	bulletPhysicsData.shape = new btBoxShape(btVector3(dimensions.x, dimensions.y, dimensions.z));
	
	btTransform boxShapeTransform;
	boxShapeTransform.setIdentity();
	boxShapeTransform.setOrigin(btVector3());
	
	btScalar mass(1.0f);
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	bulletPhysicsData.shape->calculateLocalInertia(mass, localInertia);
	
	bulletPhysicsData.motionState = new BulletMotionState(boxShapeTransform, std::move(motionStateListener));
	btRigidBody::btRigidBodyConstructionInfo boxInfo(mass, bulletPhysicsData.motionState, bulletPhysicsData.shape, localInertia);
	//boxInfo.m_friction = 1.0f;
	//boxInfo.m_restitution = 1.0f;
	bulletPhysicsData.rigidBody = new btRigidBody(boxInfo);
	
	dynamicsWorld_->addRigidBody(bulletPhysicsData.rigidBody);
	
	bulletPhysicsData_.push_back(bulletPhysicsData);
	auto index = bulletPhysicsData_.size() - 1;
	
	return CollisionShapeHandle(index);
}

void PhysicsEngine::rotation(const CollisionShapeHandle& collisionShapeHandle, const glm::quat& orientation)
{
	const auto id = collisionShapeHandle.getId();
	
	auto& bulletPhysicsData = bulletPhysicsData_[id];
	
	btTransform transform = bulletPhysicsData.motionState->getWorldTransform();
	
	const btQuaternion quat = btQuaternion(orientation.x, orientation.y, orientation.z, orientation.w);
	transform.setRotation(quat);
	
	bulletPhysicsData.rigidBody->setWorldTransform(transform);
}

void PhysicsEngine::position(const CollisionShapeHandle& collisionShapeHandle, const float32 x, const float32 y, const float32 z)
{
	const auto id = collisionShapeHandle.getId();
	
	auto& bulletPhysicsData = bulletPhysicsData_[id];
	
	btTransform transform = bulletPhysicsData.motionState->getWorldTransform();
	
	const btVector3 pos = btVector3(x, y, z);
	transform.setOrigin(pos);
	
	bulletPhysicsData.rigidBody->setWorldTransform(transform);
}

void PhysicsEngine::position(const CollisionShapeHandle& collisionShapeHandle, const glm::vec3& position)
{
	const auto id = collisionShapeHandle.getId();
	
	auto& bulletPhysicsData = bulletPhysicsData_[id];

	btTransform transform = bulletPhysicsData.motionState->getWorldTransform();
	
	const btVector3 pos = btVector3(position.x, position.y, position.z);
	transform.setOrigin(pos);
	
	bulletPhysicsData.rigidBody->setWorldTransform(transform);
}

}
}
}
