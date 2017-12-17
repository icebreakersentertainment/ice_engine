#include "physics/PhysicsFactory.hpp"
#include "physics/bullet/PhysicsEngine.hpp"

namespace ice_engine
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

std::unique_ptr<IPhysicsEngine> PhysicsFactory::createPhysicsEngine(
	utilities::Properties* properties,
	fs::IFileSystem* fileSystem,
	logger::ILogger* logger
)
{
	auto ptr = std::unique_ptr< IPhysicsEngine >( new bullet::PhysicsEngine(properties, fileSystem, logger) );
	
	return std::move( ptr );
}

}
}
