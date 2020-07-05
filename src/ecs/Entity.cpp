#include "ecs/Entity.hpp"

#include "Scene.hpp"

namespace ice_engine
{
namespace ecs
{

const std::string& SceneDelegate::name() const
{
	return scene_->name();
}

pathfinding::CrowdHandle SceneDelegate::createCrowd(const pathfinding::NavigationMeshHandle& navigationMeshHandle, const pathfinding::CrowdConfig& crowdConfig)
{
	return scene_->createCrowd(navigationMeshHandle, crowdConfig);
}

void SceneDelegate::destroy(const pathfinding::CrowdHandle& crowdHandle)
{
	scene_->destroy(crowdHandle);
}

pathfinding::AgentHandle SceneDelegate::createAgent(const pathfinding::CrowdHandle& crowdHandle, const glm::vec3& position, const pathfinding::AgentParams& agentParams)
{
	return scene_->createAgent(crowdHandle, position, agentParams);
}

void SceneDelegate::destroy(const pathfinding::CrowdHandle& crowdHandle, const pathfinding::AgentHandle& agentHandle)
{
	scene_->destroy(crowdHandle, agentHandle);
}

physics::RigidBodyObjectHandle SceneDelegate::createRigidBodyObject(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return scene_->createRigidBodyObject(collisionShapeHandle);
}

physics::RigidBodyObjectHandle SceneDelegate::createRigidBodyObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const float32 mass,
	const float32 friction,
	const float32 restitution
)
{
	return scene_->createRigidBodyObject(
		collisionShapeHandle,
		mass,
		friction,
		restitution
	);
}

physics::RigidBodyObjectHandle SceneDelegate::createRigidBodyObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 mass,
	const float32 friction,
	const float32 restitution
)
{
	return scene_->createRigidBodyObject(
		collisionShapeHandle,
		position,
		orientation,
		mass,
		friction,
		restitution
	);
}

void SceneDelegate::destroy(const physics::RigidBodyObjectHandle& rigidBodyObjectHandle)
{
	scene_->destroy(rigidBodyObjectHandle);
}

physics::GhostObjectHandle SceneDelegate::createGhostObject(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return scene_->createGhostObject(collisionShapeHandle);
}

physics::GhostObjectHandle SceneDelegate::createGhostObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation
)
{
	return scene_->createGhostObject(
		collisionShapeHandle,
		position,
		orientation
	);
}

void SceneDelegate::destroy(const physics::GhostObjectHandle& ghostObjectHandle)
{
	scene_->destroy(ghostObjectHandle);
}


graphics::RenderableHandle SceneDelegate::createRenderable(
	const ModelHandle& modelHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const glm::vec3& scale
)
{
	return scene_->createRenderable(
		modelHandle,
		position,
		orientation,
		scale
	);
}

graphics::RenderableHandle SceneDelegate::createRenderable(
	const graphics::MeshHandle& meshHandle,
	const graphics::TextureHandle& textureHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const glm::vec3& scale
)
{
	return scene_->createRenderable(
		meshHandle,
		textureHandle,
		position,
		orientation,
		scale
	);
}

void SceneDelegate::destroy(const graphics::RenderableHandle& renderableHandle)
{
	scene_->destroy(renderableHandle);
}

graphics::PointLightHandle SceneDelegate::createPointLight(const glm::vec3& position)
{
	return scene_->createPointLight(position);
}

void SceneDelegate::destroy(const graphics::PointLightHandle& pointLightHandle)
{
	scene_->destroy(pointLightHandle);
}


graphics::BonesHandle SceneDelegate::createBones(const uint32 maxNumberOfBones)
{
	return scene_->createBones(maxNumberOfBones);
}

void SceneDelegate::destroy(const graphics::BonesHandle& bonesHandle)
{
	scene_->destroy(bonesHandle);
}


void SceneDelegate::attach(const graphics::RenderableHandle& renderableHandle, const graphics::BonesHandle& bonesHandle)
{
	scene_->attach(renderableHandle, bonesHandle);
}

void SceneDelegate::detach(const graphics::RenderableHandle& renderableHandle, const graphics::BonesHandle& bonesHandle)
{
	scene_->detach(renderableHandle, bonesHandle);
}


void SceneDelegate::attachBoneAttachment(
	const graphics::RenderableHandle& renderableHandle,
	const graphics::BonesHandle& bonesHandle,
	const glm::ivec4& boneIds,
	const glm::vec4& boneWeights
)
{
	scene_->attachBoneAttachment(
		renderableHandle,
		bonesHandle,
		boneIds,
		boneWeights
	);
}

void SceneDelegate::attachBoneAttachment(
	const graphics::RenderableHandle& renderableHandle,
	const graphics::BonesHandle& bonesHandle,
	const graphics::MeshHandle meshHandle,
	const std::string& boneName,
	const glm::ivec4& boneIds,
	const glm::vec4& boneWeights
)
{
	scene_->attachBoneAttachment(
		renderableHandle,
		bonesHandle,
		meshHandle,
		boneName,
		boneIds,
		boneWeights
	);
}

void SceneDelegate::detachBoneAttachment(const graphics::RenderableHandle& renderableHandle)
{
	scene_->detachBoneAttachment(renderableHandle);
}

graphics::TerrainRenderableHandle SceneDelegate::createTerrainRenderable(const graphics::TerrainHandle terrainHandle)
{
	return scene_->createTerrainRenderable(terrainHandle);
}

void SceneDelegate::destroy(const graphics::TerrainRenderableHandle& terrainRenderableHandle)
{
	scene_->destroy(terrainRenderableHandle);
}

graphics::SkyboxRenderableHandle SceneDelegate::createSkyboxRenderable(const graphics::SkyboxHandle skyboxHandle)
{
	return scene_->createSkyboxRenderable(skyboxHandle);
}

void SceneDelegate::destroy(const graphics::SkyboxRenderableHandle& skyboxRenderableHandle)
{
	scene_->destroy(skyboxRenderableHandle);
}


pathfinding::IPathfindingEngine& SceneDelegate::pathfindingEngine() const
{
	return scene_->pathfindingEngine();
}

// need to forward declare this so that we can use it below
template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent>();

template <>
entityx::ComponentHandle<ParentComponent> Entity::assign<ParentComponent, Entity>(Entity&& entity)
{
	std::cout << "1: " << entity_ << std::endl;

	entityx::ComponentHandle<ParentComponent> componentHandle;

	if (entity_.has_component<ParentComponent>()) entity_.remove<ParentComponent>();

	std::cout << "PARENT C" << std::endl;
	std::cout << "entity " << Entity(entity).id() << std::endl;
	std::cout << "PARENT C" << std::endl;
	componentHandle = entity_.assign<ParentComponent>(std::forward<Entity>(entity));
	std::cout << "entity " << componentHandle->entity << std::endl;
	if (componentHandle->entity)
	{
		auto childrenComponent = componentHandle->entity.component<ChildrenComponent>();

		if (!childrenComponent) childrenComponent = componentHandle->entity.assign<ChildrenComponent>();

		if (std::find(childrenComponent->children.begin(), childrenComponent->children.end(), *this) == childrenComponent->children.end())
		{
			childrenComponent->children.push_back(*this);
		}
	}

	return componentHandle;
}

template <>
entityx::ComponentHandle<ParentComponent> Entity::assign<ParentComponent, Entity&&>(Entity&& entity)
{
	std::cout << entity_ << std::endl;
	return assign<ParentComponent, Entity>(std::forward<Entity>(entity));
}

template <>
entityx::ComponentHandle<ParentComponent> Entity::assign<ParentComponent, Entity&>(Entity& entity)
{
	std::cout << "PARENT C 2" << std::endl;
		std::cout << entity << std::endl;
	return assign<ParentComponent>(Entity(entity));
}

template <>
entityx::ComponentHandle<ParentComponent> Entity::assign<ParentComponent, const Entity&>(const Entity& entity)
{
	std::cout << "PARENT C 3" << std::endl;
		std::cout << entity << std::endl;
	return assign<ParentComponent>(Entity(entity));
}

template <>
entityx::ComponentHandle<ParentComponent> Entity::assign<ParentComponent, ParentComponent>(ParentComponent&& parentComponent)
{
	std::cout << "PARENT C 4" << std::endl;
		std::cout << parentComponent.entity << std::endl;
	return assign<ParentComponent>(parentComponent.entity);
}

template <>
entityx::ComponentHandle<ParentComponent> Entity::assign<ParentComponent, ParentComponent&&>(ParentComponent&& parentComponent)
{
	std::cout << "PARENT C 5" << std::endl;
		std::cout << parentComponent.entity << std::endl;
	return assign<ParentComponent>(parentComponent.entity);
}

template <>
entityx::ComponentHandle<ParentComponent> Entity::assign<ParentComponent, ParentComponent&>(ParentComponent& parentComponent)
{
	std::cout << "PARENT C 6" << std::endl;
		std::cout << parentComponent.entity << std::endl;
	return assign<ParentComponent>(parentComponent.entity);
}

template <>
entityx::ComponentHandle<ParentComponent> Entity::assign<ParentComponent, const ParentComponent&>(const ParentComponent& parentComponent)
{
	std::cout << "PARENT C 7" << std::endl;
			std::cout << parentComponent.entity << std::endl;
	return assign<ParentComponent>(parentComponent.entity);
}

template <>
entityx::ComponentHandle<ParentComponent> Entity::assign<ParentComponent>()
{
	return assign<ParentComponent>(Entity());
}

template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent, std::vector<Entity>>(std::vector<Entity>&& children)
{
	std::cout << entity_ << std::endl;

	entityx::ComponentHandle<ChildrenComponent> componentHandle;

	if (entity_.has_component<ChildrenComponent>()) entity_.remove<ChildrenComponent>();

	std::cout << "CHILDREN C" << std::endl;
	componentHandle = entity_.assign<ChildrenComponent>(std::forward<std::vector<Entity>>(children));

	for (auto& childEntity : componentHandle->children)
	{
		childEntity.assign<ParentComponent>(*this);
	}

	return componentHandle;
}

template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent, std::vector<Entity>&&>(std::vector<Entity>&& children)
{
	return assign<ChildrenComponent, std::vector<Entity>>(std::forward<std::vector<Entity>>(children));
}

template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent, std::vector<Entity>&>(std::vector<Entity>& children)
{
	return assign<ChildrenComponent>(children);
}

template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent, const std::vector<Entity>&>(const std::vector<Entity>& children)
{
	return assign<ChildrenComponent>(children);
}

template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent, ChildrenComponent>(ChildrenComponent&& childrenComponent)
{
	return assign<ChildrenComponent>(childrenComponent.children);
}

template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent, ChildrenComponent&&>(ChildrenComponent&& childrenComponent)
{
	return assign<ChildrenComponent>(childrenComponent.children);
}

template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent, ChildrenComponent&>(ChildrenComponent& childrenComponent)
{
	return assign<ChildrenComponent>(childrenComponent.children);
}

template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent, const ChildrenComponent&>(const ChildrenComponent& childrenComponent)
{
	return assign<ChildrenComponent>(childrenComponent.children);
}

template <>
entityx::ComponentHandle<ChildrenComponent> Entity::assign<ChildrenComponent>()
{
	return assign<ChildrenComponent>(std::vector<Entity>());
}

entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assignParentBoneAttachmentComponent(std::string boneName, glm::ivec4 boneIds, glm::vec4 boneWeights)
{
	std::cout << entity_ << std::endl;

	entityx::ComponentHandle<ParentBoneAttachmentComponent> componentHandle;

	auto gc = entity_.component<ecs::GraphicsComponent>();
	if (!gc) throw std::runtime_error("cannot assign parent bone attachment component - graphics component does not exist");
	if (!gc->renderableHandle) throw std::runtime_error("cannot assign parent bone attachment component - renderableHandle is not valid");

	auto pc = entity_.component<ParentComponent>();
	if (!pc) throw std::runtime_error("cannot assign parent bone attachment component - parent component does not exist");
	if (!pc->entity) throw std::runtime_error("cannot assign parent bone attachment component - entity is not valid");

	auto pgc = pc->entity.component<GraphicsComponent>();
	if (!pgc) throw std::runtime_error("cannot assign parent bone attachment component - parent graphics component does not exist");
	if (!pgc->meshHandle) throw std::runtime_error("cannot assign parent bone attachment component - meshHandle is not valid");

	auto pac = pc->entity.component<AnimationComponent>();
	if (!pac) throw std::runtime_error("cannot assign parent bone attachment component - parent animation component does not exist");
	if (!pac->bonesHandle) throw std::runtime_error("cannot assign parent bone attachment component - bonesHandle is not valid");

	if (entity_.has_component<ParentBoneAttachmentComponent>()) entity_.remove<ParentBoneAttachmentComponent>();

	std::cout << "P BONE ATTACHMENT C" << std::endl;
	componentHandle = entity_.assign<ParentBoneAttachmentComponent>(
		std::forward<std::string>(boneName),
		std::forward<glm::ivec4>(boneIds),
		std::forward<glm::vec4>(boneWeights)
	);

	scene_->attachBoneAttachment(
		gc->renderableHandle,
		pac->bonesHandle,
		pgc->meshHandle,
		componentHandle->boneName,
		componentHandle->boneIds,
		componentHandle->boneWeights
	);

	return componentHandle;
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, std::string, glm::ivec4, glm::vec4>(std::string&& boneName, glm::ivec4&& boneIds, glm::vec4&& boneWeights)
{
	return assignParentBoneAttachmentComponent(boneName, boneIds, boneWeights);
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, std::string&, glm::ivec4&, glm::vec4&>(std::string& boneName, glm::ivec4& boneIds, glm::vec4& boneWeights)
{
	return assignParentBoneAttachmentComponent(boneName, boneIds, boneWeights);
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, const std::string&, const glm::ivec4&, const glm::vec4&>(const std::string& boneName, const glm::ivec4& boneIds, const glm::vec4& boneWeights)
{
	return assign<ParentBoneAttachmentComponent>(boneName, boneIds, boneWeights);
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, std::string&&, glm::ivec4&&, glm::vec4&&>(std::string&& boneName, glm::ivec4&& boneIds, glm::vec4&& boneWeights)
{
	return assign<ParentBoneAttachmentComponent>(boneName, boneIds, boneWeights);
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, std::string>(std::string&& boneName)
{
	return assign<ParentBoneAttachmentComponent>(std::forward<std::string>(boneName), glm::ivec4(), glm::vec4());
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, std::string&&>(std::string&& boneName)
{
	return assign<ParentBoneAttachmentComponent>(std::forward<std::string>(boneName), glm::ivec4(), glm::vec4());
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, std::string&>(std::string& boneName)
{
	return assign<ParentBoneAttachmentComponent>(std::string(boneName));
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, const std::string&>(const std::string& boneName)
{
	return assign<ParentBoneAttachmentComponent>(std::string(boneName));
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, ParentBoneAttachmentComponent>(ParentBoneAttachmentComponent&& parentBoneAttachmentComponent)
{
	return assign<ParentBoneAttachmentComponent>(
		parentBoneAttachmentComponent.boneName,
		parentBoneAttachmentComponent.boneIds,
		parentBoneAttachmentComponent.boneWeights
	);
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, ParentBoneAttachmentComponent&&>(ParentBoneAttachmentComponent&& parentBoneAttachmentComponent)
{
	return assign<ParentBoneAttachmentComponent>(
		parentBoneAttachmentComponent.boneName,
		parentBoneAttachmentComponent.boneIds,
		parentBoneAttachmentComponent.boneWeights
	);
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent, const ParentBoneAttachmentComponent&>(const ParentBoneAttachmentComponent& parentBoneAttachmentComponent)
{
	return assign<ParentBoneAttachmentComponent>(
		parentBoneAttachmentComponent.boneName,
		parentBoneAttachmentComponent.boneIds,
		parentBoneAttachmentComponent.boneWeights
	);
}

template <>
entityx::ComponentHandle<ParentBoneAttachmentComponent> Entity::assign<ParentBoneAttachmentComponent>()
{
	return assign<ParentBoneAttachmentComponent, std::string, glm::ivec4, glm::vec4>({}, {}, {});
}

}
}
