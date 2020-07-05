#ifndef ICEENGINEENTITY_H_
#define ICEENGINEENTITY_H_

#include <boost/static_assert.hpp>

#include <glm/gtx/string_cast.hpp>

#include <entityx/entityx.h>

#include "pathfinding/IPathfindingEngine.hpp"

#include "ecs/GraphicsComponent.hpp"
#include "ecs/PointLightComponent.hpp"
#include "ecs/AnimationComponent.hpp"
#include "ecs/SkeletonComponent.hpp"
#include "ecs/GraphicsTerrainComponent.hpp"
#include "ecs/GraphicsSkyboxComponent.hpp"
#include "ecs/RigidBodyObjectComponent.hpp"
#include "ecs/GhostObjectComponent.hpp"
#include "ecs/PositionComponent.hpp"
#include "ecs/OrientationComponent.hpp"
#include "ecs/PathfindingAgentComponent.hpp"
#include "ecs/PathfindingObstacleComponent.hpp"
#include "ecs/PathfindingCrowdComponent.hpp"
//#include "ecs/PathfindingComponent.hpp"
//#include "ecs/ScriptObjectComponent.hpp"
#include "ecs/DirtyComponent.hpp"
#include "ecs/ParentBoneAttachmentComponent.hpp"

#include "ModelHandle.hpp"

#include "serialization/std/Bitset.hpp"
#include "serialization/SplitMember.hpp"

#include "exceptions/RuntimeException.hpp"

namespace ice_engine
{

class Scene;

namespace ecs
{

class ParentComponent;
class ChildrenComponent;

class SceneDelegate
{
public:
	SceneDelegate(Scene* scene = nullptr) : scene_(scene)
	{
	}

	const std::string& name() const;

	pathfinding::CrowdHandle createCrowd(const pathfinding::NavigationMeshHandle& navigationMeshHandle, const pathfinding::CrowdConfig& crowdConfig);
	void destroy(const pathfinding::CrowdHandle& crowdHandle);

	pathfinding::AgentHandle createAgent(const pathfinding::CrowdHandle& crowdHandle, const glm::vec3& position, const pathfinding::AgentParams& agentParams = pathfinding::AgentParams());
	void destroy(const pathfinding::CrowdHandle& crowdHandle, const pathfinding::AgentHandle& agentHandle);

	physics::RigidBodyObjectHandle createRigidBodyObject(const physics::CollisionShapeHandle& collisionShapeHandle);
	physics::RigidBodyObjectHandle createRigidBodyObject(
		const physics::CollisionShapeHandle& collisionShapeHandle,
		const float32 mass,
		const float32 friction,
		const float32 restitution
	);
	physics::RigidBodyObjectHandle createRigidBodyObject(
		const physics::CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const float32 mass = 1.0f,
		const float32 friction = 1.0f,
		const float32 restitution = 1.0f
	);
	void destroy(const physics::RigidBodyObjectHandle& rigidBodyObjectHandle);
	physics::GhostObjectHandle createGhostObject(const physics::CollisionShapeHandle& collisionShapeHandle);
	physics::GhostObjectHandle createGhostObject(
		const physics::CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation
	);
	void destroy(const physics::GhostObjectHandle& ghostObjectHandle);

	graphics::RenderableHandle createRenderable(
		const ModelHandle& modelHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const glm::vec3& scale = glm::vec3(1.0f)
	);
	graphics::RenderableHandle createRenderable(
		const graphics::MeshHandle& meshHandle,
		const graphics::TextureHandle& textureHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const glm::vec3& scale = glm::vec3(1.0f)
	);
	void destroy(const graphics::RenderableHandle& renderableHandle);

	graphics::PointLightHandle createPointLight(const glm::vec3& position);
	void destroy(const graphics::PointLightHandle& pointLightHandle);

	graphics::BonesHandle createBones(const uint32 maxNumberOfBones);
	void destroy(const graphics::BonesHandle& bonesHandle);

	void attach(const graphics::RenderableHandle& renderableHandle, const graphics::BonesHandle& bonesHandle);
	void detach(const graphics::RenderableHandle& renderableHandle, const graphics::BonesHandle& bonesHandle);

	void attachBoneAttachment(
		const graphics::RenderableHandle& renderableHandle,
		const graphics::BonesHandle& bonesHandle,
		const glm::ivec4& boneIds,
		const glm::vec4& boneWeights
	);

	void attachBoneAttachment(
		const graphics::RenderableHandle& renderableHandle,
		const graphics::BonesHandle& bonesHandle,
		const graphics::MeshHandle meshHandle,
		const std::string& boneName,
		const glm::ivec4& boneIds,
		const glm::vec4& boneWeights
	);

	void detachBoneAttachment(const graphics::RenderableHandle& renderableHandle);

	graphics::TerrainRenderableHandle createTerrainRenderable(const graphics::TerrainHandle terrainHandle);
	void destroy(const graphics::TerrainRenderableHandle& terrainRenderableHandle);

	graphics::SkyboxRenderableHandle createSkyboxRenderable(const graphics::SkyboxHandle skyboxHandle);
	void destroy(const graphics::SkyboxRenderableHandle& skyboxRenderableHandle);

	pathfinding::IPathfindingEngine& pathfindingEngine() const;

private:
	Scene* scene_ = nullptr;
};

class Entity
{
public:
	Entity() = default;

	Entity(entityx::Entity::Id id) : entity_(nullptr, id)
	{
	}

	Entity(Scene* scene, entityx::Entity entity) : scene_(scene), sceneDelegate_({scene}), entity_(entity)
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
    	  return entity_.valid() && scene_ != nullptr;
      }

      void invalidate()
      {
         	  entity_.invalidate();
         	 scene_ = nullptr;
      }

      Scene* scene() const
      {
         	return scene_;
      }

      entityx::Entity::Id id() const { return entity_.id(); }

      template <typename C>
      typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ParentComponent>>::value, entityx::ComponentHandle<C>>::type
      assign(const ParentComponent& parentComponent);

        template <typename C, typename ... Args>
        typename std::enable_if<
			!std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GraphicsComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<AnimationComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PointLightComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GraphicsTerrainComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GraphicsSkyboxComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<RigidBodyObjectComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GhostObjectComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PathfindingCrowdComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PathfindingAgentComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PathfindingObstacleComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ParentComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ChildrenComponent>>::value
			&& !std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ParentBoneAttachmentComponent>>::value
			, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        	//std::cout << entity_ << std::endl;
          return entity_.assign<C>(std::forward<Args>(args) ...);
        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ParentComponent>>::value, entityx::ComponentHandle<C>>::type
		assign(Args && ... args);
//        {
//        	//static_assert(0);
////        	BOOST_STATIC_ASSERT(0);
//        	return {};
//        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ChildrenComponent>>::value, entityx::ComponentHandle<C>>::type
		assign(Args && ... args);
//        {
//        	//static_assert(0);
//        	//        	BOOST_STATIC_ASSERT(0);
//			return {};
//        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ParentBoneAttachmentComponent>>::value, entityx::ComponentHandle<C>>::type
		assign(Args && ... args);
//        {
//        	//static_assert(0);
//        	//        	BOOST_STATIC_ASSERT(0);
//			return {};
//        }

//        template <typename C, typename ... Args>
//        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ParentBoneAttachmentComponent>>::value, entityx::ComponentHandle<C>>::type
//		assign(Args & ... args);
//        {
//        	//static_assert(0);
//        	//        	BOOST_STATIC_ASSERT(0);
//			return {};
//        }

//        template <typename C, typename ... Args>
//        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ChildrenComponent>>::value, entityx::ComponentHandle<C>>::type
//        assign();
//
//        template <typename C, typename ... Args>
//        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ChildrenComponent>>::value, entityx::ComponentHandle<C>>::type
//        assign(std::vector<Entity> children);

//        template <typename C, typename ... Args>
//        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<ParentBoneAttachmentComponent>>::value, entityx::ComponentHandle<C>>::type
//        assign(std::string&& boneName = {},  glm::ivec4&& boneIds = {}, glm::vec4&& boneWeights = {});

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GraphicsComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        	std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>()) entity_.remove<C>();

        	std::cout << "GCGCGCGC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->renderableHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
        		auto oc = entity_.component<OrientationComponent>();
        		componentHandle->renderableHandle = sceneDelegate_.createRenderable(componentHandle->meshHandle, componentHandle->textureHandle, pc->position, oc->orientation, componentHandle->scale);
        	}

        	return componentHandle;
        }

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<AnimationComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        	std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	auto gc = entity_.component<ecs::GraphicsComponent>();

        	std::cout << "ACACACAC 1" << std::endl;
        	if (!gc) throw RuntimeException("cannot assign animation component - graphics component does not exist");
        	std::cout << "ACACACAC 2" << std::endl;
        	if (!gc->renderableHandle) throw RuntimeException("cannot assign animation component - renderableHandle is not valid");
        	std::cout << "ACACACAC 3" << std::endl;

        	if (entity_.has_component<C>()) entity_.remove<C>();

        	std::cout << "ACACACAC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->bonesHandle)
        	{
        		componentHandle->bonesHandle = sceneDelegate_.createBones(100);
        		sceneDelegate_.attach(gc->renderableHandle, componentHandle->bonesHandle);
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
        		if (c->pointLightHandle) sceneDelegate_.destroy(c->pointLightHandle);
        	}

        	std::cout << "PL" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->pointLightHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
        		componentHandle->pointLightHandle = sceneDelegate_.createPointLight(pc->position);
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
        		if (c->terrainRenderableHandle) sceneDelegate_.destroy(c->terrainRenderableHandle);
        	}

        	std::cout << "GTC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->terrainRenderableHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
        		componentHandle->terrainRenderableHandle = sceneDelegate_.createTerrainRenderable(
        			componentHandle->terrainHandle
				);
        	}

        	return componentHandle;
        }

				template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<GraphicsSkyboxComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        	std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>())
        	{
        		auto c = entity_.component<C>();
        		if (c->skyboxRenderableHandle) sceneDelegate_.destroy(c->skyboxRenderableHandle);
        	}

        	std::cout << "GSC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->skyboxRenderableHandle)
        	{
        		// auto pc = entity_.component<PositionComponent>();
        		componentHandle->skyboxRenderableHandle = sceneDelegate_.createSkyboxRenderable(
        			componentHandle->skyboxHandle
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

        	if (entity_.has_component<C>()) entity_.remove<C>();

        	std::cout << "PCPCPCPC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->rigidBodyObjectHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
				auto oc = entity_.component<OrientationComponent>();

        		componentHandle->rigidBodyObjectHandle = sceneDelegate_.createRigidBodyObject(
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

        	if (entity_.has_component<C>()) entity_.remove<C>();

        	std::cout << "GHC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->ghostObjectHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
        		        		auto oc = entity_.component<OrientationComponent>();

        		componentHandle->ghostObjectHandle = sceneDelegate_.createGhostObject(
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

        	if (entity_.has_component<C>()) entity_.remove<C>();

        	std::cout << "CC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->crowdHandle)
        	{
        		std::cout << "e: "<< componentHandle->crowdConfig.maxAgents << std::endl;

        		auto pc = entity_.component<PositionComponent>();
				auto oc = entity_.component<OrientationComponent>();

        		componentHandle->crowdHandle = sceneDelegate_.createCrowd(
					componentHandle->navigationMeshHandle,
					componentHandle->crowdConfig
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

        	if (entity_.has_component<C>()) entity_.remove<C>();

        	std::cout << "PAC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->agentHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();
				auto oc = entity_.component<OrientationComponent>();

				std::cout << "PAC: " << glm::to_string(pc->position) << std::endl;
        		componentHandle->agentHandle = sceneDelegate_.createAgent(
					componentHandle->crowdHandle,
					pc->position,
					componentHandle->agentParams
				);
        		std::cout << "PAC: " << glm::to_string(pc->position) << std::endl;
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

        template <typename C, typename ... Args>
        typename std::enable_if<std::is_same<entityx::ComponentHandle<C>, entityx::ComponentHandle<PathfindingObstacleComponent>>::value, entityx::ComponentHandle<C>>::type
        assign(Args && ... args)
		{
        		std::cout << entity_ << std::endl;

        	entityx::ComponentHandle<C> componentHandle;

        	if (entity_.has_component<C>()) entity_.remove<C>();

        	std::cout << "POC" << std::endl;
        	componentHandle = entity_.assign<C>(std::forward<Args>(args) ...);

        	if (!componentHandle->obstacleHandle)
        	{
        		auto pc = entity_.component<PositionComponent>();

        		componentHandle->obstacleHandle = sceneDelegate_.pathfindingEngine().createObstacle(
					componentHandle->polygonMeshHandle,
					pc->position,
					componentHandle->radius,
					componentHandle->height
				);
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
					scene_ = nullptr;
				}

    	friend std::ostream& operator<<(std::ostream& os, const Entity& other)
    	{
    		os << "Entity(Entity: " << other.entity_ << ", Scene: " << (other.scene_ != nullptr ? other.sceneDelegate_.name() : "") << ")";
    		return os;
    	}

private:
    entityx::Entity entity_;
    SceneDelegate sceneDelegate_;
    Scene* scene_ = nullptr;

	entityx::ComponentHandle<ParentBoneAttachmentComponent> assignParentBoneAttachmentComponent(std::string boneName, glm::ivec4 boneIds, glm::vec4 boneWeights);
};

}
}

#include "ecs/ParentComponent.hpp"
#include "ecs/ChildrenComponent.hpp"
//#include "Scene.hpp"

#endif /* ICEENGINEENTITY_H_ */
