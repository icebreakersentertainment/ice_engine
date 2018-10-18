#ifndef ENTITYCOMPONENTSYSTEM_H_
#define ENTITYCOMPONENTSYSTEM_H_

#include <entityx/entityx.h>
#include <entityx/deps/Dependencies.h>

#include "ecs/Entity.hpp"
#include "ecs/GraphicsComponent.hpp"
#include "ecs/RigidBodyObjectComponent.hpp"
#include "ecs/GhostObjectComponent.hpp"
#include "ecs/PositionComponent.hpp"
#include "ecs/OrientationComponent.hpp"
#include "ecs/PathfindingAgentComponent.hpp"
#include "ecs/PathfindingObstacleComponent.hpp"
#include "ecs/PathfindingCrowdComponent.hpp"
#include "ecs/PointLightComponent.hpp"
#include "ecs/ScriptObjectComponent.hpp"
#include "ecs/DirtyComponent.hpp"
#include "ecs/PersistableComponent.hpp"

#include "serialization/std/Bitset.hpp"
#include "serialization/SplitMember.hpp"

namespace ice_engine
{

class Scene;

namespace ecs
{

template <bool All>
using ViewIteratorDelegateIterator = typename entityx::EntityManager::BaseView<All>::Iterator;

template <bool All>
using ViewIteratorIterator = entityx::EntityManager::ViewIterator<ViewIteratorDelegateIterator<All>, All>;

template <bool All = false>
class ViewIterator
{
public:
	ViewIterator(Scene* scene, entityx::EntityManager::ViewIterator<typename entityx::EntityManager::BaseView<All>::Iterator, All> viewIterator) : scene_(scene), viewIterator_(viewIterator)
	{
	}

	ViewIterator& operator++()
	{
		++viewIterator_;

		return *this;
    }

    bool operator==(const ViewIterator& rhs) const
    {
    	return *static_cast<const ViewIteratorDelegateIterator<All>*>(&viewIterator_) == *static_cast<const ViewIteratorDelegateIterator<All>*>(&rhs.viewIterator_);
    }

    bool operator!=(const ViewIterator& rhs) const
    {
    	return *static_cast<const ViewIteratorDelegateIterator<All>*>(&viewIterator_) != *static_cast<const ViewIteratorDelegateIterator<All>*>(&rhs.viewIterator_);
    }

    Entity operator*()
    {
    	return Entity(scene_, *viewIterator_);
    }

    const Entity operator*() const
    {
    	return Entity(scene_, *viewIterator_);
    }

private:
    Scene* scene_;
    ViewIteratorIterator<All> viewIterator_;
};

template <bool All = false>
class EcsView
{
public:
	EcsView(Scene* scene, entityx::EntityManager::BaseView<All> baseView) : scene_(scene), baseView_(baseView) {}
	~EcsView() = default;

	class Iterator : public ViewIterator<All>
	{
	public:
		Iterator(
			Scene* scene,
			ViewIteratorIterator<All> viewIterator) : ViewIterator<All>(scene, viewIterator)
		{
		}
	};

	Iterator begin() { return Iterator(scene_, baseView_.begin()); }
	Iterator end() { return Iterator(scene_, baseView_.end()); }
	const Iterator begin() const { return Iterator(scene_, baseView_.begin()); }
	const Iterator end() const { return Iterator(scene_, baseView_.end()); }

private:
	Scene* scene_;
	entityx::EntityManager::BaseView<All> baseView_;
};

class EntityComponentSystem
{
public:
	explicit EntityComponentSystem(Scene* scene) : scene_(scene), entities_(events_), systems_(entities_, events_)
	{
		systems_.add<entityx::deps::Dependency<GraphicsComponent, PositionComponent>>();
		systems_.add<entityx::deps::Dependency<GraphicsComponent, OrientationComponent>>();
		systems_.add<entityx::deps::Dependency<RigidBodyObjectComponent, PositionComponent>>();
		systems_.add<entityx::deps::Dependency<RigidBodyObjectComponent, OrientationComponent>>();
		systems_.add<entityx::deps::Dependency<GhostObjectComponent, PositionComponent>>();
		systems_.add<entityx::deps::Dependency<GhostObjectComponent, OrientationComponent>>();
		systems_.add<entityx::deps::Dependency<PointLightComponent, PositionComponent>>();
		systems_.configure();
	}

	EcsView<true> entities()
	{
		auto debugView = entities_.entities_for_debugging();

		return EcsView<true>(scene_, debugView);
	}

	const EcsView<true> entities() const
	{
		auto debugView = const_cast<EntityComponentSystem*>(this)->entities_.entities_for_debugging();

		return EcsView<true>(scene_, debugView);
	}

	~EntityComponentSystem() = default;
	
	template<class T, typename Receiver>
	void subscribe(Receiver& receiver)
	{
		events_.subscribe<T>(receiver);
	}
	
	Entity create()
	{
		return Entity(scene_, entities_.create());
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
	
	Entity get(entityx::Entity::Id id)
	{
		return Entity(scene_, entities_.get(id));
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
	EcsView<false> entitiesWithComponents()
	{
		auto view = entities_.entities_with_components<C ...>();

		return EcsView<false>(scene_, view);
	}

	template <typename ... C>
	const EcsView<false> entitiesWithComponents() const
	{
		auto view = const_cast<EntityComponentSystem*>(this)->entities_.entities_with_components<C ...>();

		return EcsView<false>(scene_, view);
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
	friend class boost::serialization::access;

	Scene* scene_;
	entityx::EventManager events_;
	entityx::EntityManager entities_;
	entityx::SystemManager systems_;

	entityx::EntityManager::ComponentMask generateEntityMask(const ice_engine::ecs::Entity& entity) const
	{
		entityx::EntityManager::ComponentMask mask;

		if (entity.hasComponent<ice_engine::ecs::GhostObjectComponent>())			mask.set(ice_engine::ecs::GhostObjectComponent::id());
		if (entity.hasComponent<ice_engine::ecs::GraphicsComponent>())				mask.set(ice_engine::ecs::GraphicsComponent::id());
		if (entity.hasComponent<ice_engine::ecs::SkeletonComponent>())				mask.set(ice_engine::ecs::SkeletonComponent::id());
		if (entity.hasComponent<ice_engine::ecs::AnimationComponent>())				mask.set(ice_engine::ecs::AnimationComponent::id());
		if (entity.hasComponent<ice_engine::ecs::GraphicsTerrainComponent>())		mask.set(ice_engine::ecs::GraphicsTerrainComponent::id());
		if (entity.hasComponent<ice_engine::ecs::PathfindingAgentComponent>())		mask.set(ice_engine::ecs::PathfindingAgentComponent::id());
		if (entity.hasComponent<ice_engine::ecs::PathfindingObstacleComponent>())	mask.set(ice_engine::ecs::PathfindingObstacleComponent::id());
		if (entity.hasComponent<ice_engine::ecs::PathfindingCrowdComponent>())		mask.set(ice_engine::ecs::PathfindingCrowdComponent::id());
		if (entity.hasComponent<ice_engine::ecs::PointLightComponent>())			mask.set(ice_engine::ecs::PointLightComponent::id());
		if (entity.hasComponent<ice_engine::ecs::PositionComponent>())				mask.set(ice_engine::ecs::PositionComponent::id());
		if (entity.hasComponent<ice_engine::ecs::OrientationComponent>())			mask.set(ice_engine::ecs::OrientationComponent::id());
		if (entity.hasComponent<ice_engine::ecs::RigidBodyObjectComponent>())		mask.set(ice_engine::ecs::RigidBodyObjectComponent::id());
		if (entity.hasComponent<ice_engine::ecs::ScriptObjectComponent>())			mask.set(ice_engine::ecs::ScriptObjectComponent::id());
		if (entity.hasComponent<ice_engine::ecs::ParentComponent>())				mask.set(ice_engine::ecs::ParentComponent::id());
		if (entity.hasComponent<ice_engine::ecs::ChildrenComponent>())				mask.set(ice_engine::ecs::ChildrenComponent::id());
		if (entity.hasComponent<ice_engine::ecs::ParentBoneAttachmentComponent>())	mask.set(ice_engine::ecs::ParentBoneAttachmentComponent::id());

		return mask;
	}

	template<class Archive, class C>
	void saveComponent(Archive& ar, const ice_engine::ecs::Entity& entity, const unsigned int version) const
	{
		auto c = entity.component<const C>();

		ar & *c;
	}

	template<class Archive>
	void saveEntity(Archive& ar, const ice_engine::ecs::Entity& entity, const unsigned int version) const
	{
		auto mask = generateEntityMask(entity);
		//auto mask = componentMask(entity);

		ar & entity.id().index() & mask;

		if (entity.hasComponent<ice_engine::ecs::GhostObjectComponent>()) saveComponent<Archive, ice_engine::ecs::GhostObjectComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::GraphicsComponent>()) saveComponent<Archive, ice_engine::ecs::GraphicsComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::SkeletonComponent>()) saveComponent<Archive, ice_engine::ecs::SkeletonComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::AnimationComponent>()) saveComponent<Archive, ice_engine::ecs::AnimationComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::GraphicsTerrainComponent>()) saveComponent<Archive, ice_engine::ecs::GraphicsTerrainComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::PathfindingAgentComponent>()) saveComponent<Archive, ice_engine::ecs::PathfindingAgentComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::PathfindingObstacleComponent>()) saveComponent<Archive, ice_engine::ecs::PathfindingObstacleComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::PathfindingCrowdComponent>()) saveComponent<Archive, ice_engine::ecs::PathfindingCrowdComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::PointLightComponent>()) saveComponent<Archive, ice_engine::ecs::PointLightComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::PositionComponent>()) saveComponent<Archive, ice_engine::ecs::PositionComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::OrientationComponent>()) saveComponent<Archive, ice_engine::ecs::OrientationComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::RigidBodyObjectComponent>()) saveComponent<Archive, ice_engine::ecs::RigidBodyObjectComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::ScriptObjectComponent>()) saveComponent<Archive, ice_engine::ecs::ScriptObjectComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::ParentComponent>()) saveComponent<Archive, ice_engine::ecs::ParentComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::ChildrenComponent>()) saveComponent<Archive, ice_engine::ecs::ChildrenComponent>(ar, entity, version);
		if (entity.hasComponent<ice_engine::ecs::ParentBoneAttachmentComponent>()) saveComponent<Archive, ice_engine::ecs::ParentBoneAttachmentComponent>(ar, entity, version);
	}

	template<class Archive>
	void saveEntities(Archive& ar, const unsigned int version) const
	{
		for (auto entity : entitiesWithComponents<ice_engine::ecs::PersistableComponent>())
		{
			saveEntity(ar, entity, version);
		}
	}


	template<class Archive, class C>
	void loadComponent(Archive& ar, ice_engine::ecs::Entity& entity, const unsigned int version)
	{
		C c;
		ar & c;

		entities_.assign<C>(entity.id(), c);
	}

	template<class Archive>
	void loadEntity(Archive& ar, ice_engine::ecs::Entity& entity, const entityx::EntityManager::ComponentMask mask, const unsigned int version)
	{

		if (mask.test(ice_engine::ecs::GhostObjectComponent::id())) loadComponent<Archive, ice_engine::ecs::GhostObjectComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::GraphicsComponent::id())) loadComponent<Archive, ice_engine::ecs::GraphicsComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::SkeletonComponent::id())) loadComponent<Archive, ice_engine::ecs::SkeletonComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::AnimationComponent::id())) loadComponent<Archive, ice_engine::ecs::AnimationComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::GraphicsTerrainComponent::id())) loadComponent<Archive, ice_engine::ecs::GraphicsTerrainComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::PathfindingAgentComponent::id())) loadComponent<Archive, ice_engine::ecs::PathfindingAgentComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::PathfindingObstacleComponent::id())) loadComponent<Archive, ice_engine::ecs::PathfindingObstacleComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::PathfindingCrowdComponent::id())) loadComponent<Archive, ice_engine::ecs::PathfindingCrowdComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::PointLightComponent::id())) loadComponent<Archive, ice_engine::ecs::PointLightComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::PositionComponent::id())) loadComponent<Archive, ice_engine::ecs::PositionComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::OrientationComponent::id())) loadComponent<Archive, ice_engine::ecs::OrientationComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::RigidBodyObjectComponent::id())) loadComponent<Archive, ice_engine::ecs::RigidBodyObjectComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::ScriptObjectComponent::id())) loadComponent<Archive, ice_engine::ecs::ScriptObjectComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::ParentComponent::id())) loadComponent<Archive, ice_engine::ecs::ParentComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::ChildrenComponent::id())) loadComponent<Archive, ice_engine::ecs::ChildrenComponent>(ar, entity, version);
		if (mask.test(ice_engine::ecs::ParentBoneAttachmentComponent::id())) loadComponent<Archive, ice_engine::ecs::ParentBoneAttachmentComponent>(ar, entity, version);

		entity.assign<ice_engine::ecs::PersistableComponent>();
	}

	template<class Archive>
	void loadEntities(Archive& ar, const ice_engine::uint32 numEntities, const unsigned int version)
	{
		try {
			std::vector<ice_engine::ecs::Entity> emptyEntities;
			for (int i=0; i < numEntities; ++i)
			{
				ice_engine::uint32 oldIndex;
						entityx::EntityManager::ComponentMask mask;

						ar & oldIndex & mask;

				auto entity = create();
				while (entity.id().index() < oldIndex)
				{
					emptyEntities.push_back(entity);
					entity = create();
				}

				loadEntity(ar, entity, mask, version);
			}

			for (auto& entity : emptyEntities)
			{
				entity.destroy();
			}
		} catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		boost::serialization::split_member(ar, *this, version);
	}

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		ice_engine::uint32 numEntities = 0;
		for (auto entity : entitiesWithComponents<ice_engine::ecs::PersistableComponent>())
		{
			++numEntities;
		}

		ar & numEntities;

		saveEntities(ar, version);
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		ice_engine::uint32 numEntities = 0;
		ar & numEntities;

		loadEntities(ar, numEntities, version);
	}
};

}
}

#endif /* ENTITYCOMPONENTSYSTEM_H_ */
