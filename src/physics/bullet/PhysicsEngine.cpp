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

CollisionShapeHandle PhysicsEngine::createStaticPlane(const glm::vec3& planeNormal, const float32 planeConstant, entities::Entity entity, IGameEngine* gameEngine)
{
	//auto groundShape = std::make_unique<btStaticPlaneShape>(btVector3(planeNormal.x, planeNormal.y, planeNormal.z), planeConstant);
	btStaticPlaneShape* groundShape = new btStaticPlaneShape(btVector3(planeNormal.x, planeNormal.y, planeNormal.z), planeConstant);
	
	btTransform groundShapeTransform;
	groundShapeTransform.setIdentity();
	groundShapeTransform.setOrigin(btVector3(0.0f, -6.0f, 0.0f));
	
	BulletMotionState* groundMotionState = new BulletMotionState(groundShapeTransform, entity, gameEngine);
	//btRigidBody::btRigidBodyConstructionInfo rbGroundInfo(0, groundMotionState, groundShape.get());
	btRigidBody::btRigidBodyConstructionInfo rbGroundInfo(0, groundMotionState, groundShape);
	rbGroundInfo.m_friction = 1.0f;
	rbGroundInfo.m_restitution = 1.0f;
	btRigidBody* rigidBodyGround = new btRigidBody(rbGroundInfo);
	
	dynamicsWorld_->addRigidBody(rigidBodyGround);
	
	bulletMotionStates_.push_back(groundMotionState);
	rigidBodies_.push_back(rigidBodyGround);
	auto index = rigidBodies_.size() - 1;
	
	return CollisionShapeHandle(index);
}

CollisionShapeHandle PhysicsEngine::createBoxShape(const glm::vec3& dimensions, entities::Entity entity, IGameEngine* gameEngine)
{
	//auto groundShape = std::make_unique<btStaticPlaneShape>(btVector3(planeNormal.x, planeNormal.y, planeNormal.z), planeConstant);
	btCollisionShape* boxShape = new btBoxShape(btVector3(dimensions.x, dimensions.y, dimensions.z));
	//btCollisionShape* boxShape = new btSphereShape(btScalar(1.));
	
	btTransform boxShapeTransform;
	boxShapeTransform.setIdentity();
	boxShapeTransform.setOrigin(btVector3(1.0f, 0.0f, 1.0f));
	
	btScalar mass(1.0f);
	btVector3 localInertia(0.0f, 0.0f, 0.0f);
	boxShape->calculateLocalInertia(mass, localInertia);
	
	BulletMotionState* boxMotionState = new BulletMotionState(boxShapeTransform, entity, gameEngine);
	//btDefaultMotionState* boxMotionState = new btDefaultMotionState(boxShapeTransform);
	//btRigidBody::btRigidBodyConstructionInfo rbGroundInfo(0, boxMotionState, groundShape.get());
	btRigidBody::btRigidBodyConstructionInfo boxInfo(mass, boxMotionState, boxShape, localInertia);
	//boxInfo.m_friction = 1.0f;
	//boxInfo.m_restitution = 1.0f;
	btRigidBody* boxRigidBody = new btRigidBody(boxInfo);
	
	dynamicsWorld_->addRigidBody(boxRigidBody);
	
	bulletMotionStates_.push_back(boxMotionState);
	rigidBodies_.push_back(boxRigidBody);
	auto index = rigidBodies_.size() - 1;
	
	return CollisionShapeHandle(index);
}

}
}
}
