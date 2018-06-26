#include "EntityComponentSystemEventListener.hpp"

#include "Scene.hpp"

namespace ice_engine
{

EntityComponentSystemEventListener::EntityComponentSystemEventListener(Scene& scene, ecs::EntityComponentSystem& entityComponentSystem)
	:
	scene_(scene),
	entityComponentSystem_(entityComponentSystem)
{
	entityComponentSystem.subscribe<entityx::EntityCreatedEvent>(*this);
	entityComponentSystem.subscribe<entityx::EntityDestroyedEvent>(*this);
	entityComponentSystem.subscribe<entityx::ComponentAddedEvent<ecs::GraphicsComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::GraphicsComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::RigidBodyObjectComponent>>(*this);
}

void EntityComponentSystemEventListener::receive(const entityx::EntityCreatedEvent& event)
{

}

void EntityComponentSystemEventListener::receive(const entityx::EntityDestroyedEvent& event)
{

}

void EntityComponentSystemEventListener::receive(const entityx::ComponentAddedEvent<ecs::GraphicsComponent>& event)
{

}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::GraphicsComponent>& event)
{
	if (event.component->renderableHandle) scene_.destroy(event.component->renderableHandle);
}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::RigidBodyObjectComponent>& event)
{
	if (event.component->rigidBodyObjectHandle) scene_.destroy(event.component->rigidBodyObjectHandle);
}

}
