#ifndef ENTITYCOMPONENTSYSTEMEVENTLISTENER_H_
#define ENTITYCOMPONENTSYSTEMEVENTLISTENER_H_

#include "ecs/EntityComponentSystem.hpp"

namespace ice_engine
{

class Scene;

class EntityComponentSystemEventListener : public entityx::Receiver<EntityComponentSystemEventListener>
{
public:
	EntityComponentSystemEventListener(Scene& scene, ecs::EntityComponentSystem& entityComponentSystem);
	
	void receive(const entityx::EntityCreatedEvent& event);
	void receive(const entityx::EntityDestroyedEvent& event);
	void receive(const entityx::ComponentAddedEvent<ecs::GraphicsComponent>& event);
	void receive(const entityx::ComponentRemovedEvent<ecs::GraphicsComponent>& event);
	void receive(const entityx::ComponentRemovedEvent<ecs::RigidBodyObjectComponent>& event);

private:
	Scene& scene_;
	ecs::EntityComponentSystem& entityComponentSystem_;
};

}

#endif /* ENTITYCOMPONENTSYSTEMEVENTLISTENER_H_ */
