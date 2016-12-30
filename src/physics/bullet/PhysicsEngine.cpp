#include <exception>

#include "logger/Logger.hpp"

#include "physics/bullet/PhysicsEngine.hpp"
#include "physics/bullet/StaticPlaneShape.hpp"

namespace physics
{
namespace bullet
{

PhysicsEngine::PhysicsEngine()
{
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

void PhysicsEngine::tick(glm::detail::float32 delta)
{
	dynamicsWorld_->stepSimulation(delta, 10);
}

ICollisionShape* PhysicsEngine::createStaticPlane(const glm::vec3& planeNormal, glm::detail::float32 planeConstant)
{
	 auto shape = std::unique_ptr<ICollisionShape>( new StaticPlaneShape(planeNormal, planeConstant) );
	 
	 auto retValue = shape.get();
	 
	 shapes_.push_back( std::move(shape) );
	 
	 return retValue;
}

}
}
