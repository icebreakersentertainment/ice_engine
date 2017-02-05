#include "physics/PhysicsFactory.hpp"
#include "physics/bullet/PhysicsEngine.hpp"

namespace hercules
{
namespace physics
{

PhysicsFactory::PhysicsFactory()
{
}

PhysicsFactory::PhysicsFactory(const PhysicsFactory& other)
{
}

PhysicsFactory::~PhysicsFactory()
{
}

std::unique_ptr<IPhysicsEngine> PhysicsFactory::createPhysicsEngine()
{
	auto ptr = std::unique_ptr< IPhysicsEngine >( new bullet::PhysicsEngine() );
	
	return std::move( ptr );
}

}
}
