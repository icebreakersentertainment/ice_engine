#include <iostream>

#include "entities/Entity.hpp"

namespace hercules
{
namespace entities
{

Entity::Entity()
{
	std::cout << "Created Entity (no entityx entity passed in)" << std::endl;
}

Entity::Entity(entityx::Entity entity) : entity_(entity)
{
	std::cout << "Created Entity (passed in entityx entity)" << std::endl;
}

Entity::~Entity()
{
}

void Entity::addComponent(GraphicsComponent* graphicsComponent)
{
	graphicsComponent->addRef();
	
	//entityx::ptr<GraphicsComponent> ptr = entityx::ptr<GraphicsComponent>( graphicsComponent );
	std::cout << "assigning from copy" << std::endl;
	//entity_.assign_from_copy(*graphicsComponent);
}

void Entity::removeComponent(GraphicsComponent* graphicsComponent)
{
	// TODO: Deal with releasing ref (when it destroys the object) and also destroying the shared_ptr from entityx
	//graphicsComponent->releaseRef();
	
	//entity_.remove<GraphicsComponent>();
}
	
}
}
