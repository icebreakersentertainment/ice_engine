#ifndef ENTITYCOMPONENTSYSTEM_H_
#define ENTITYCOMPONENTSYSTEM_H_

#include <entityx/entityx.h>
#include <entityx/deps/Dependencies.h>

#include "ecs/GraphicsComponent.hpp"
#include "ecs/RigidBodyObjectComponent.hpp"
#include "ecs/GhostObjectComponent.hpp"
#include "ecs/PositionOrientationComponent.hpp"

namespace ice_engine
{
namespace ecs
{

class EntityComponentSystem
{
public:
	EntityComponentSystem() : entities_(events_), systems_(entities_, events_)
	{
		systems_.add<entityx::deps::Dependency<GraphicsComponent, PositionOrientationComponent>>();
		systems_.add<entityx::deps::Dependency<RigidBodyObjectComponent, PositionOrientationComponent>>();
		systems_.add<entityx::deps::Dependency<GhostObjectComponent, PositionOrientationComponent>>();
		systems_.configure();
	}
	
	~EntityComponentSystem() = default;
	
	template<class T, typename Receiver>
	void subscribe(Receiver& receiver)
	{
		events_.subscribe<T>(receiver);
	}
	
	entityx::Entity create()
	{
		return entities_.create();
	}
	
	void destroy(entityx::Entity::Id entity)
	{
		entities_.destroy(entity);
	}
	
	bool valid(entityx::Entity::Id id) const
	{
		return entities_.valid(id);
	}
	
	entityx::Entity::Id createId(uint32 index) const
	{
		return entities_.create_id(index);
	}
	
	entityx::Entity get(entityx::Entity::Id id)
	{
		return entities_.get(id);
	}

	template <typename C, typename = typename std::enable_if<!std::is_const<C>::value>::type>
	entityx::ComponentHandle<C> component(entityx::Entity::Id id)
	{
		return entities_.component<C>(id);
	}
	
	template <typename C, typename = typename std::enable_if<std::is_const<C>::value>::type>
	const entityx::ComponentHandle<C, const entityx::EntityManager> component(entityx::Entity::Id id) const
	{
		return entities_.component<C>(id);
	}
	
	template <typename C>
	bool hasComponent(entityx::Entity::Id id) const
	{
		return entities_.has_component<C>(id);
	}
	
	template <typename C, typename ... Args>
	entityx::ComponentHandle<C> assign(entityx::Entity::Id id, Args && ... args)
	{
		return entities_.assign<C>(id, std::forward<Args>(args) ...);
	}
	
	template <typename ... C>
	entityx::EntityManager::View<C ...> entitiesWithComponents()
	{
		return entities_.entities_with_components<C ...>();
	}

	template <typename C>
	void remove(entityx::Entity::Id id)
	{
		entities_.remove<C>(id);
	}
	
	uint32 numEntities() const
	{
		return entities_.size();
	}

private:
	entityx::EventManager events_;
	entityx::EntityManager entities_;
	entityx::SystemManager systems_;
};

}
}

#endif /* ENTITYCOMPONENTSYSTEM_H_ */
