#ifndef ICEENGINEENTITY_H_
#define ICEENGINEENTITY_H_

namespace ice_engine
{
namespace ecs
{
class Entity;
}
}

#include <glm/gtx/string_cast.hpp>

#include <entityx/entityx.h>

#include "ecs/GraphicsComponent.hpp"
#include "ecs/PointLightComponent.hpp"
#include "ecs/GraphicsTerrainComponent.hpp"
#include "ecs/RigidBodyObjectComponent.hpp"
#include "ecs/GhostObjectComponent.hpp"
#include "ecs/PositionComponent.hpp"
#include "ecs/OrientationComponent.hpp"
#include "ecs/PathfindingAgentComponent.hpp"
#include "ecs/PathfindingCrowdComponent.hpp"
//#include "ecs/PathfindingComponent.hpp"
//#include "ecs/ScriptObjectComponent.hpp"
#include "ecs/DirtyComponent.hpp"

#include "serialization/std/Bitset.hpp"
#include "serialization/SplitMember.hpp"

namespace ice_engine
{

class Scene;

namespace ecs
{

class Entity
{
public:
	Entity() = default;

	Entity(Scene* scene, entityx::Entity entity) : scene_(scene), entity_(entity)
	{
	}

	Entity(const Entity& other) = default;
	Entity &operator = (const Entity& other) = default;

    operator bool() const {
        return valid();
      }

      bool operator == (const Entity& other) const
    {
        return other.entity_ == entity_;
      }

      bool operator != (const Entity& other) const {
        return !(other.entity_ == entity_);
      }

      bool operator < (const Entity &other) const {
        return other.entity_ < entity_;
      }

      bool valid() const
      {
    	  return entity_.valid();
      }

      void invalidate()
      {
         	  entity_.invalidate();
      }

      Scene* scene() const
      {
         	return scene_;
      }

      entityx::Entity::Id id() const { return entity_.id(); }

        template <typename C, typename ... Args>
        typename std::enable_if<
			!std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GraphicsComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PointLightComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GraphicsTerrainComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<RigidBodyObjectComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GhostObjectComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PathfindingCrowdComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PathfindingAgentComponent>>::value
			, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        	//std::cout << entity_ << std::endl;
          return entity_.assign<C>(std::forward<Args>(args) ...);
        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GraphicsComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        	std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>())
        	{
        		auto c = entity_.component<C>();
        		if (c->renderableHandle) scene_->destroy(c->renderableHandle);
        	}

        	std::cout << "GCGCGCGC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->renderableHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
        		auto oc = entity_.component<OrientationComponent>();
        		componentHandle->renderableHandle = scene_->createRenderable(componentHandle->modelHandle, pc->position, oc->orientation, componentHandle->scale);
        	}

        	return componentHandle;
        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PointLightComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        	std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>())
        	{
        		auto c = entity_.component<C>();
        		if (c->pointLightHandle) scene_->destroy(c->pointLightHandle);
        	}

        	std::cout << "PL" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->pointLightHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
        		componentHandle->pointLightHandle = scene_->createPointLight(pc->position);
        	}

        	return componentHandle;
        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GraphicsTerrainComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        	std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>())
        	{
        		auto c = entity_.component<C>();
        		if (c->terrainRenderableHandle) scene_->destroy(c->terrainRenderableHandle);
        	}

        	std::cout << "GTC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->terrainRenderableHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
        		componentHandle->terrainRenderableHandle = scene_->createTerrainRenderable(
        			componentHandle->terrainHandle
				);
        	}

        	return componentHandle;
        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<RigidBodyObjectComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        		std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>())
        	{
        		auto c = entity_.component<C>();
        		if (c->rigidBodyObjectHandle) scene_->destroy(c->rigidBodyObjectHandle);
        	}

        	std::cout << "PCPCPCPC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->rigidBodyObjectHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
				auto oc = entity_.component<OrientationComponent>();

        		componentHandle->rigidBodyObjectHandle = scene_->createRigidBodyObject(
					componentHandle->collisionShapeHandle,
					pc->position,
					oc->orientation,
					componentHandle->mass,
					componentHandle->friction,
					componentHandle->restitution
				);

        		if (entity_.has_component<DirtyComponent>())
        		{
        			auto dirtyComponent = entity_.component<DirtyComponent>();
        			dirtyComponent->dirty |= DirtyFlags::DIRTY_SOURCE_SCRIPT | DirtyFlags::DIRTY_RIGID_BODY_OBJECT;
        		}
        		else
        		{
        			entity_.assign<DirtyComponent>(DirtyFlags::DIRTY_SOURCE_SCRIPT | DirtyFlags::DIRTY_RIGID_BODY_OBJECT);
        		}
        	}

        	return componentHandle;
        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GhostObjectComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        		std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>())
        	{
        		auto c = entity_.component<C>();
        		if (c->ghostObjectHandle) scene_->destroy(c->ghostObjectHandle);
        	}

        	std::cout << "GHC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->ghostObjectHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
        		        		auto oc = entity_.component<OrientationComponent>();

        		componentHandle->ghostObjectHandle = scene_->createGhostObject(
					componentHandle->collisionShapeHandle,
					pc->position,
					oc->orientation
				);

        		if (entity_.has_component<DirtyComponent>())
        		{
        			auto dirtyComponent = entity_.component<DirtyComponent>();
        			dirtyComponent->dirty |= DirtyFlags::DIRTY_SOURCE_SCRIPT | DirtyFlags::DIRTY_GHOST_OBJECT;
        		}
        		else
        		{
        			entity_.assign<DirtyComponent>(DirtyFlags::DIRTY_SOURCE_SCRIPT | DirtyFlags::DIRTY_GHOST_OBJECT);
        		}
        	}

        	return componentHandle;
        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PathfindingCrowdComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        		std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>())
        	{
        		auto c = entity_.component<C>();
        		if (c->crowdHandle) scene_->destroy(c->crowdHandle);
        	}

        	std::cout << "CC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->crowdHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
				auto oc = entity_.component<OrientationComponent>();

        		componentHandle->crowdHandle = scene_->createCrowd(
					componentHandle->navigationMeshHandle
				);
        	}

        	return componentHandle;
        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PathfindingAgentComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        		std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>())
        	{
        		auto c = entity_.component<C>();
        		if (c->agentHandle) scene_->destroy(c->crowdHandle, c->agentHandle);
        	}

        	std::cout << "PAC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->agentHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
				auto oc = entity_.component<OrientationComponent>();

				std::cout << "PAC: " << glm::to_string(pc->position) << std::endl;
        		componentHandle->agentHandle = scene_->createAgent(
					componentHandle->crowdHandle,
					pc->position,
					componentHandle->agentParams
				);

        		if (entity_.has_component<DirtyComponent>())
        		{
        			auto dirtyComponent = entity_.component<DirtyComponent>();
        			dirtyComponent->dirty |= DirtyFlags::DIRTY_SOURCE_SCRIPT | DirtyFlags::DIRTY_PATHFINDING_AGENT;
        		}
        		else
        		{
        			entity_.assign<DirtyComponent>(DirtyFlags::DIRTY_SOURCE_SCRIPT | DirtyFlags::DIRTY_PATHFINDING_AGENT);
        		}
        	}

        	return componentHandle;
        }

        template <typename C>
        entityx::ComponentHandle<C> assignFromCopy(const C &component)
		{
          return entity_.assign_from_copy<C>(std::forward<const C &>(component));
        }

        template <typename C, typename ... Args>
        entityx::ComponentHandle<C> replace(Args && ... args)
		{
          return entity_.replace<C>(std::forward<Args>(args) ...);
        }

        template <typename C>
        void remove() {
          entity_.remove<C>();
        }

        template <typename C, typename = typename std::enable_if<!std::is_const<C>::value>::type>
        entityx::ComponentHandle<C> component() {
          return entity_.component<C>();
        }

        template <typename C, typename = typename std::enable_if<std::is_const<C>::value>::type>
        const entityx::ComponentHandle<C, const entityx::EntityManager> component() const
		{
          return entity_.component<C>();
        }

        template <typename ... Components>
        std::tuple<entityx::ComponentHandle<Components>...> components() {
          return entity_.components<Components...>();
        }

        template <typename ... Components>
        std::tuple<entityx::ComponentHandle<const Components, const entityx::EntityManager>...> components() const
		{
          return entity_.components<Components...>();
        }


        template <typename C>
        bool hasComponent() const
        {
          return entity_.has_component<C>();
        }

        template <typename A, typename ... Args>
        void unpack(entityx::ComponentHandle<A> &a, entityx::ComponentHandle<Args> & ... args)
        {
          entity_.unpack(a, args ...);
        }

        void destroy()
        {
          entity_.destroy();
        }

private:
    entityx::Entity entity_;
    Scene* scene_;
};

}
}

#include "Scene.hpp"

#endif /* ICEENGINEENTITY_H_ */
