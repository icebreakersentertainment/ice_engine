#include <exception>

#include "logger/ILogger.hpp"

#include "physics/bullet/PhysicsEngine.hpp"
#include "physics/bullet/StaticPlaneShape.hpp"

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
	
	broadphase_ = std::unique_ptr<btBroadphaseInterface>(new btDbvtBroadphase());
	collisionConfiguration_ = std::unique_ptr<btDefaultCollisionConfiguration>(new btDefaultCollisionConfiguration());
	dispatcher_ = std::unique_ptr<btCollisionDispatcher>(new btCollisionDispatcher(collisionConfiguration_.get()));
	solver_ = std::unique_ptr<btSequentialImpulseConstraintSolver>(new btSequentialImpulseConstraintSolver());
	dynamicsWorld_ = std::unique_ptr<btDiscreteDynamicsWorld>(new btDiscreteDynamicsWorld(dispatcher_.get(), broadphase_.get(), solver_.get(), collisionConfiguration_.get()));
	
	dynamicsWorld_->setGravity(btVector3(0, -10, 0));
}

PhysicsEngine::PhysicsEngine(const PhysicsEngine& other)
{
}

PhysicsEngine::~PhysicsEngine()
{
}

void PhysicsEngine::tick(float32 delta)
{
	dynamicsWorld_->stepSimulation(delta, 10);
}

ICollisionShape* PhysicsEngine::createStaticPlane(const glm::vec3& planeNormal, float32 planeConstant)
{
	 auto shape = std::unique_ptr<ICollisionShape>( new StaticPlaneShape(planeNormal, planeConstant) );
	 
	 auto retValue = shape.get();
	 
	 shapes_.push_back( std::move(shape) );
	 
	 return retValue;
}

}
}
}
