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
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::AnimationComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::RigidBodyObjectComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::GhostObjectComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::PathfindingCrowdComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::PathfindingAgentComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::PathfindingObstacleComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::ParentComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::ChildrenComponent>>(*this);
	entityComponentSystem.subscribe<entityx::ComponentRemovedEvent<ecs::ParentBoneAttachmentComponent>>(*this);
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

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::AnimationComponent>& event)
{
	if (event.component->bonesHandle)
	{
		const auto gc = event.entity.component<const ecs::GraphicsComponent>();

		if (gc && gc->renderableHandle) scene_.detach(gc->renderableHandle, event.component->bonesHandle);

		scene_.destroy(event.component->bonesHandle);
	}
}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::RigidBodyObjectComponent>& event)
{
	if (event.component->rigidBodyObjectHandle) scene_.destroy(event.component->rigidBodyObjectHandle);
}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::GhostObjectComponent>& event)
{
	if (event.component->ghostObjectHandle) scene_.destroy(event.component->ghostObjectHandle);
}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::PathfindingCrowdComponent>& event)
{
	if (event.component->crowdHandle) scene_.destroy(event.component->crowdHandle);
}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::PathfindingAgentComponent>& event)
{
	if (event.component->agentHandle) scene_.destroy(event.component->crowdHandle, event.component->agentHandle);
}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::PathfindingObstacleComponent>& event)
{
	if (event.component->obstacleHandle) scene_.pathfindingEngine().destroy(event.component->polygonMeshHandle, event.component->obstacleHandle);
}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::ParentComponent>& event)
{
	if (event.component->entity)
	{
		auto e = event.component->entity;
		auto childrenComponent = e.component<ecs::ChildrenComponent>();
		childrenComponent->children.erase(
			std::remove(
				childrenComponent->children.begin(),
				childrenComponent->children.end(),
				event.component->entity
			),
			childrenComponent->children.end()
		);
	}
}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::ChildrenComponent>& event)
{
	auto children = event.component->children;
	for (auto& entity : children)
	{
		entity.destroy();
	}

//	while (!event.component->children.empty())
//	{
//		ecs::Entity e = event.component->children.back();
//		event.component->children.pop_back();
//
//		e.destroy();
//	}
}

void EntityComponentSystemEventListener::receive(const entityx::ComponentRemovedEvent<ecs::ParentBoneAttachmentComponent>& event)
{
	const auto gc = event.entity.component<const ecs::GraphicsComponent>();

	if (gc && gc->renderableHandle) scene_.detachBoneAttachment(gc->renderableHandle);
}

}
