#include <chrono>

#include <glm/gtx/string_cast.hpp>

#include "Scene.hpp"

#include "IceEngineMotionChangeListener.hpp"

namespace ice_engine
{

Scene::Scene(
		const std::string& name,
		IGameEngine* gameEngine,
		graphics::IGraphicsEngine* graphicsEngine,
		physics::IPhysicsEngine* physicsEngine,
		pathfinding::IPathfindingEngine* pathfindingEngine,
		scripting::IScriptingEngine* scriptingEngine,
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger,
		IThreadPool* threadPool,
		IOpenGlLoader* openGlLoader
	)
	:
		name_(name),
		gameEngine_(gameEngine),
		graphicsEngine_(graphicsEngine),
		physicsEngine_(physicsEngine),
		pathfindingEngine_(pathfindingEngine),
		scriptingEngine_(scriptingEngine),
		scriptObjectHandle_(nullptr),
		properties_(properties),
		fileSystem_(fileSystem),
		logger_(logger),
		threadPool_(threadPool),
		openGlLoader_(openGlLoader)
{
	initialize();
}

Scene::~Scene()
{
	destroy();
}

void Scene::initialize()
{
	renderSceneHandle_ = graphicsEngine_->createRenderScene();
	physicsSceneHandle_ = physicsEngine_->createPhysicsScene();
	pathfindingSceneHandle_ = pathfindingEngine_->createPathfindingScene();
	executionContextHandle_ = scriptingEngine_->createExecutionContext();
}

void Scene::destroy()
{
	graphicsEngine_->destroyRenderScene(renderSceneHandle_);
	physicsEngine_->destroyPhysicsScene(physicsSceneHandle_);
	pathfindingEngine_->destroyPathfindingScene(pathfindingSceneHandle_);
	scriptingEngine_->destroyExecutionContext(executionContextHandle_);
	
	if (scriptObjectHandle_)
	{
		scriptingEngine_->releaseScriptObject(scriptObjectHandle_);
	}
}

void Scene::tick(const float32 delta)
{
	scripting::ParameterList params;
	params.add(delta);
	
	if (scriptObjectHandle_)
	{
		scriptingEngine_->execute(scriptObjectHandle_, std::string("void preTick(const float)"), params, executionContextHandle_);
	}
	
	auto beginPhysicsTime = std::chrono::high_resolution_clock::now();
		
	physicsEngine_->tick(physicsSceneHandle_, delta);
	
	auto endPhysicsTime = std::chrono::high_resolution_clock::now();
	
	sceneStatistics_.physicsTime = std::chrono::duration<float32>(endPhysicsTime - beginPhysicsTime).count();
	
	pathfindingEngine_->tick(pathfindingSceneHandle_, delta);
	
	if (scriptObjectHandle_)
	{
		scriptingEngine_->execute(scriptObjectHandle_, std::string("void postTick(const float)"), params, executionContextHandle_);
	}
}

void Scene::render()
{
	graphicsEngine_->render(renderSceneHandle_);
}

void Scene::setISceneThingyInstance(scripting::ScriptObjectHandle scriptObjectHandle)
{
	scriptObjectHandle_ = scriptObjectHandle;
}

physics::CollisionShapeHandle Scene::createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant)
{
	return physicsEngine_->createStaticPlaneShape(planeNormal, planeConstant);
}

physics::CollisionShapeHandle Scene::createStaticBoxShape(const glm::vec3& dimensions)
{
	return physicsEngine_->createStaticBoxShape(dimensions);
}

void Scene::destroyStaticShape(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return physicsEngine_->destroyStaticShape(collisionShapeHandle);
}

void Scene::destroyAllStaticShapes()
{
	return physicsEngine_->destroyAllStaticShapes();
}

physics::RigidBodyObjectHandle Scene::createDynamicRigidBodyObject(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return physicsEngine_->createDynamicRigidBodyObject(physicsSceneHandle_, collisionShapeHandle);
}

physics::RigidBodyObjectHandle Scene::createDynamicRigidBodyObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const float32 mass,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createDynamicRigidBodyObject(physicsSceneHandle_, collisionShapeHandle, mass, friction, restitution);
}

physics::RigidBodyObjectHandle Scene::createDynamicRigidBodyObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 mass,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createDynamicRigidBodyObject(physicsSceneHandle_, collisionShapeHandle, position, orientation, mass, friction, restitution);
}

physics::RigidBodyObjectHandle Scene::createStaticRigidBodyObject(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return physicsEngine_->createStaticRigidBodyObject(physicsSceneHandle_, collisionShapeHandle);
}

physics::RigidBodyObjectHandle Scene::createStaticRigidBodyObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createStaticRigidBodyObject(physicsSceneHandle_, collisionShapeHandle, friction, restitution);
}

physics::RigidBodyObjectHandle Scene::createStaticRigidBodyObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createStaticRigidBodyObject(physicsSceneHandle_, collisionShapeHandle, position, orientation, friction, restitution);
}

physics::GhostObjectHandle Scene::createGhostObject(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return physicsEngine_->createGhostObject(physicsSceneHandle_, collisionShapeHandle);
}

physics::GhostObjectHandle Scene::createGhostObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation
)
{
	return physicsEngine_->createGhostObject(physicsSceneHandle_, collisionShapeHandle, position, orientation);
}

void Scene::addMotionChangeListener(const entities::Entity& entity)
{
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	std::unique_ptr<IceEngineMotionChangeListener> motionChangeListener = std::make_unique<IceEngineMotionChangeListener>(entity, this);
	
	physicsEngine_->setMotionChangeListener(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, std::move(motionChangeListener));
}

void Scene::removeMotionChangeListener(const entities::Entity& entity)
{
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	physicsEngine_->setMotionChangeListener(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, nullptr);
}

void Scene::addUserData(const entities::Entity& entity, const entities::RigidBodyObjectComponent& rigidBodyObjectComponent)
{
	physics::UserData userData;
	userData.value(entity);
	physicsEngine_->setUserData(physicsSceneHandle_, rigidBodyObjectComponent.rigidBodyObjectHandle, userData);
}

void Scene::addUserData(const entities::Entity& entity, const entities::GhostObjectComponent& ghostObjectComponent)
{
	physics::UserData userData;
	userData.value(entity);
	physicsEngine_->setUserData(physicsSceneHandle_, ghostObjectComponent.ghostObjectHandle, userData);
}

void Scene::removeUserData(const entities::Entity& entity, const entities::RigidBodyObjectComponent& rigidBodyObjectComponent)
{
	physicsEngine_->setUserData(physicsSceneHandle_, rigidBodyObjectComponent.rigidBodyObjectHandle, physics::UserData());
}

void Scene::removeUserData(const entities::Entity& entity, const entities::GhostObjectComponent& ghostObjectComponent)
{
	physicsEngine_->setUserData(physicsSceneHandle_, ghostObjectComponent.ghostObjectHandle, physics::UserData());
}

graphics::RenderableHandle Scene::createRenderable(const ModelHandle& modelHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name)
{
	return gameEngine_->createRenderable(renderSceneHandle_, modelHandle, shaderProgramHandle, name);
}

graphics::RenderableHandle Scene::createRenderable(const graphics::MeshHandle& meshHandle, const graphics::TextureHandle& textureHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name)
{
	return gameEngine_->createRenderable(renderSceneHandle_, meshHandle, textureHandle, shaderProgramHandle, name);
}

graphics::PointLightHandle Scene::createPointLight(const glm::vec3& position)
{
	return graphicsEngine_->createPointLight(renderSceneHandle_, position);
}

std::string Scene::getName() const
{
	return name_;
}

const SceneStatistics& Scene::getSceneStatistics() const
{
	return sceneStatistics_;
}

entities::Entity Scene::createEntity()
{
	entityx::Entity e = entityComponentSystem_.entities.create();
	
	logger_->debug( "Created entity with id: " + std::to_string(e.id().id()) );
	
	return entities::Entity( e.id().id() );
}

void Scene::destroyEntity(const entities::Entity& entity)
{
	entityComponentSystem_.entities.destroy(static_cast<entityx::Entity::Id>(entity.id()));
}

uint32 Scene::getNumEntities() const
{
	return entityComponentSystem_.entities.size();
}

Raycast Scene::raycast(const ray::Ray& ray)
{
	Raycast result;
	
	auto physicsRaycast = physicsEngine_->raycast(physicsSceneHandle_, ray);
	
	result.setRay(physicsRaycast.ray());
	result.setHitPointWorld(physicsRaycast.hitPointWorld());
	result.setHitNormalWorld(physicsRaycast.hitNormalWorld());
	
	entities::Entity entity;
	
	if (physicsRaycast.rigidBodyObjectHandle())
	{
		auto userData = physicsEngine_->getUserData(physicsSceneHandle_, physicsRaycast.rigidBodyObjectHandle());
		entity = userData.value<entities::Entity>();
	}
	else if (physicsRaycast.ghostObjectHandle())
	{
		auto userData = physicsEngine_->getUserData(physicsSceneHandle_, physicsRaycast.ghostObjectHandle());
		entity = userData.value<entities::Entity>();
	}
	
	result.setEntity(entity);
	
	return result;
}

void Scene::assign(const entities::Entity& entity, const entities::GraphicsComponent& component)
{
	entityComponentSystem_.entities.assign<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()), std::forward<const entities::GraphicsComponent>(component));
}

void Scene::assign(const entities::Entity& entity, const entities::RigidBodyObjectComponent& component)
{
	entityComponentSystem_.entities.assign<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()), std::forward<const entities::RigidBodyObjectComponent>(component));
	
	addUserData(entity, component);
	addMotionChangeListener(entity);
}

void Scene::assign(const entities::Entity& entity, const entities::GhostObjectComponent& component)
{
	entityComponentSystem_.entities.assign<entities::GhostObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()), std::forward<const entities::GhostObjectComponent>(component));
	
	addUserData(entity, component);
	//addMotionChangeListener(entity);
}

void Scene::assign(const entities::Entity& entity, const entities::PositionOrientationComponent& component)
{
	entityComponentSystem_.entities.assign<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()), std::forward<const entities::PositionOrientationComponent>(component));
}

void Scene::assign(const entities::Entity& entity, const entities::PointLightComponent& component)
{
	entityComponentSystem_.entities.assign<entities::PointLightComponent>(static_cast<entityx::Entity::Id>(entity.id()), std::forward<const entities::PointLightComponent>(component));
}

void Scene::rotate(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	switch( relativeTo )
	{
		case graphics::TransformSpace::TS_LOCAL:
			component->orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) ) * component->orientation;
			break;
		
		case graphics::TransformSpace::TS_WORLD:
			component->orientation =  component->orientation * glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
			break;
			
		default:
			throw std::runtime_error(std::string("Invalid TransformSpace type."));
	}
	
	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, component->orientation);
	
	if (rigidBodyObjectComponent)
	{
		physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, component->orientation);
	}
}

void Scene::rotate(const entities::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	switch( relativeTo )
	{
		case graphics::TransformSpace::TS_LOCAL:
			component->orientation = component->orientation * glm::normalize( orientation );
			break;
		
		case graphics::TransformSpace::TS_WORLD:
			component->orientation =  glm::normalize( orientation ) * component->orientation;
			break;
			
		default:
			throw std::runtime_error(std::string("Invalid TransformSpace type."));
	}
	
	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, component->orientation);
}

void Scene::rotation(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	component->orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
	
	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, component->orientation);
}

void Scene::rotation(const entities::Entity& entity, const glm::quat& orientation)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	component->orientation = glm::normalize( orientation );
	
	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, component->orientation);
}

glm::quat Scene::rotation(const entities::Entity& entity) const
{
	auto component = entityComponentSystem_.entities.component<const entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	return component->orientation;
}

void Scene::translate(const entities::Entity& entity, const glm::vec3& translate)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto pointLightComponent = entityComponentSystem_.entities.component<entities::PointLightComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	if (component)
	{
		component->position += translate;
	}
	
	if (graphicsComponent)
	{
		graphicsEngine_->position(renderSceneHandle_, graphicsComponent->renderableHandle, component->position);
	}
	
	if (rigidBodyObjectComponent)
	{
		physicsEngine_->position(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, component->position);
	}
	
	if (pointLightComponent)
	{
		pointLightComponent->position += translate;
		graphicsEngine_->position(renderSceneHandle_, pointLightComponent->pointLightHandle, pointLightComponent->position);
	}
}

void Scene::scale(const entities::Entity& entity, const float32 scale)
{
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	component->scale = glm::vec3(scale, scale, scale);
	
	graphicsEngine_->scale(renderSceneHandle_, component->renderableHandle, scale);
}

void Scene::scale(const entities::Entity& entity, const glm::vec3& scale)
{
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	component->scale = scale;
	
	graphicsEngine_->scale(renderSceneHandle_, component->renderableHandle, scale);
}

void Scene::scale(const entities::Entity& entity, const float32 x, const float32 y, const float32 z)
{
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	component->scale = glm::vec3(x, y, z);
	
	graphicsEngine_->scale(renderSceneHandle_, component->renderableHandle, x, y, z);
}

glm::vec3 Scene::scale(const entities::Entity& entity) const
{
	auto component = entityComponentSystem_.entities.component<const entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	return component->scale;
}

void Scene::lookAt(const entities::Entity& entity, const glm::vec3& lookAt)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	const glm::mat4 lookAtMatrix = glm::lookAt(component->position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	component->orientation =  glm::normalize( component->orientation * glm::quat_cast( lookAtMatrix ) );
	
	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, component->orientation);
}

void Scene::position(const entities::Entity& entity, const glm::vec3& position)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto ghostObjectComponent = entityComponentSystem_.entities.component<entities::GhostObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	component->position = position;
	
	graphicsEngine_->position(renderSceneHandle_, graphicsComponent->renderableHandle, position);
	if (rigidBodyObjectComponent)
	{
		physicsEngine_->position(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, position);
	}
	if (ghostObjectComponent)
	{
		physicsEngine_->position(physicsSceneHandle_, ghostObjectComponent->ghostObjectHandle, position);
	}
}

void Scene::position(const entities::Entity& entity, const float32 x, const float32 y, const float32 z)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	auto rigidBodyObjectComponent = entityComponentSystem_.entities.component<entities::RigidBodyObjectComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	
	component->position = glm::vec3(x, y, z);
	
	graphicsEngine_->position(renderSceneHandle_, graphicsComponent->renderableHandle, x, y, z);
	physicsEngine_->position(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, x, y, z);
}

glm::vec3 Scene::position(const entities::Entity& entity) const
{
	auto component = entityComponentSystem_.entities.component<const entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.id()));
	return component->position;
}

}
