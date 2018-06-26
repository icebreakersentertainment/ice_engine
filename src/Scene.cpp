#include <chrono>

#include <glm/gtx/string_cast.hpp>

#include "Scene.hpp"

#include "EntityComponentSystemEventListener.hpp"

#include "IceEngineMotionChangeListener.hpp"
#include "IceEnginePathfindingAgentMotionChangeListener.hpp"
#include "IceEnginePathfindingMovementRequestStateChangeListener.hpp"

namespace ice_engine
{

Scene::Scene(
		const std::string& name,
		GameEngine* gameEngine,
		audio::IAudioEngine* audioEngine,
		graphics::IGraphicsEngine* graphicsEngine,
		ITerrainFactory* terrainFactory,
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
		audioEngine_(audioEngine),
		graphicsEngine_(graphicsEngine),
		terrainFactory_(terrainFactory),
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
	audioSceneHandle_ = audioEngine_->createAudioScene();
	renderSceneHandle_ = graphicsEngine_->createRenderScene();
	physicsSceneHandle_ = physicsEngine_->createPhysicsScene();
	pathfindingSceneHandle_ = pathfindingEngine_->createPathfindingScene();
	executionContextHandle_ = scriptingEngine_->createExecutionContext();
	
	entityComponentSystemEventListener_ = std::make_unique<EntityComponentSystemEventListener>(*this, entityComponentSystem_);
}

void Scene::destroy()
{
	audioEngine_->destroyAudioScene(audioSceneHandle_);
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
	
	audioEngine_->render(audioSceneHandle_);
	
	auto beginPhysicsTime = std::chrono::high_resolution_clock::now();
		
	physicsEngine_->tick(physicsSceneHandle_, delta);
	
	auto endPhysicsTime = std::chrono::high_resolution_clock::now();
	
	sceneStatistics_.physicsTime = std::chrono::duration<float32>(endPhysicsTime - beginPhysicsTime).count();
	
	pathfindingEngine_->tick(pathfindingSceneHandle_, delta);
	
		for (auto e : entityComponentSystem_.entitiesWithComponents<ecs::ScriptObjectComponent>())
		{
			auto scriptObjectComponent = e.component<ecs::ScriptObjectComponent>();

			if (scriptObjectComponent && scriptObjectComponent->scriptObjectHandle)
			{
				scriptingEngine_->execute(scriptObjectComponent->scriptObjectHandle, std::string("void tick(const float)"), params, executionContextHandle_);
			}

		}

	if (scriptObjectHandle_)
	{
		scriptingEngine_->execute(scriptObjectHandle_, std::string("void postTick(const float)"), params, executionContextHandle_);
	}
}

void Scene::render()
{
	graphicsEngine_->render(renderSceneHandle_);
}

void Scene::setSceneThingyInstance(scripting::ScriptObjectHandle scriptObjectHandle)
{
	scriptObjectHandle_ = scriptObjectHandle;
}

void Scene::setDebugRendering(const bool enabled)
{
	debugRendering_ = enabled;
	
	physicsEngine_->setDebugRendering(physicsSceneHandle_, debugRendering_);
	pathfindingEngine_->setDebugRendering(pathfindingSceneHandle_, debugRendering_);
}

void Scene::createResources(const entityx::Entity& entity)
{
	auto positionOrientationComponent = entityComponentSystem_.component<ecs::PositionOrientationComponent>(entity.id());
	auto graphicsComponent = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	auto pointLightComponent = entityComponentSystem_.component<ecs::PointLightComponent>(entity.id());

	if (positionOrientationComponent)
	{
		if (graphicsComponent && !graphicsComponent->renderableHandle)
		{
			graphicsComponent->renderableHandle = gameEngine_->createRenderable(renderSceneHandle_, graphicsComponent->modelHandle);
			//graphicsEngine_->position(renderSceneHandle_, graphicsComponent->renderableHandle, positionOrientationComponent->position);
		}

		if (rigidBodyObjectComponent)
		{
			//physicsEngine_->position(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, positionOrientationComponent->position);
		}

		if (pointLightComponent)
		{
			//pointLightComponent->position += translate;
			//graphicsEngine_->position(renderSceneHandle_, pointLightComponent->pointLightHandle, pointLightComponent->position);
		}
	}
}

void Scene::destroyResources(const entityx::Entity& entity)
{
}

pathfinding::CrowdHandle Scene::createCrowd(const pathfinding::NavigationMeshHandle& navigationMeshHandle)
{
	return pathfindingEngine_->createCrowd(pathfindingSceneHandle_, navigationMeshHandle);
}

void Scene::destroy(const pathfinding::CrowdHandle& crowdHandle)
{
	return pathfindingEngine_->destroy(pathfindingSceneHandle_, crowdHandle);
}

pathfinding::AgentHandle Scene::createAgent(const pathfinding::CrowdHandle& crowdHandle, const glm::vec3& position, const pathfinding::AgentParams& agentParams)
{
	return pathfindingEngine_->createAgent(pathfindingSceneHandle_, crowdHandle, position, agentParams);
}

void Scene::destroy(const pathfinding::CrowdHandle& crowdHandle, const pathfinding::AgentHandle& agentHandle)
{
	return pathfindingEngine_->destroy(pathfindingSceneHandle_, crowdHandle, agentHandle);
}

void Scene::requestMoveTarget(
	const pathfinding::CrowdHandle& crowdHandle,
	const pathfinding::AgentHandle& agentHandle,
	const glm::vec3& position
)
{
	pathfindingEngine_->requestMoveTarget(pathfindingSceneHandle_, crowdHandle, agentHandle, position);
}

void Scene::requestMoveVelocity(
	const pathfinding::CrowdHandle& crowdHandle,
	const pathfinding::AgentHandle& agentHandle,
	const glm::vec3& velocity
)
{
	pathfindingEngine_->requestMoveVelocity(pathfindingSceneHandle_, crowdHandle, agentHandle, velocity);
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

physics::RigidBodyObjectHandle Scene::createRigidBodyObject(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return physicsEngine_->createRigidBodyObject(physicsSceneHandle_, collisionShapeHandle);
} 

physics::RigidBodyObjectHandle Scene::createRigidBodyObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const float32 mass,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createRigidBodyObject(physicsSceneHandle_, collisionShapeHandle, mass, friction, restitution);
}

physics::RigidBodyObjectHandle Scene::createRigidBodyObject(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 mass,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createRigidBodyObject(physicsSceneHandle_, collisionShapeHandle, position, orientation, mass, friction, restitution);
}

void Scene::destroy(const physics::RigidBodyObjectHandle& rigidBodyObjectHandle)
{
	return physicsEngine_->destroy(physicsSceneHandle_, rigidBodyObjectHandle);
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

void Scene::addMotionChangeListener(const entityx::Entity& entity)
{
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	
	std::unique_ptr<IceEngineMotionChangeListener> motionChangeListener = std::make_unique<IceEngineMotionChangeListener>(entity, this);
	
	physicsEngine_->setMotionChangeListener(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, std::move(motionChangeListener));
}

void Scene::addPathfindingAgentMotionChangeListener(const entityx::Entity& entity)
{
	auto pathfindingAgentComponent = entityComponentSystem_.component<ecs::PathfindingAgentComponent>(entity.id());
	
	std::unique_ptr<IceEnginePathfindingAgentMotionChangeListener> motionChangeListener = std::make_unique<IceEnginePathfindingAgentMotionChangeListener>(entity, this);
	
	pathfindingEngine_->setMotionChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, std::move(motionChangeListener));
}

void Scene::removeMotionChangeListener(const entityx::Entity& entity)
{
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	
	physicsEngine_->setMotionChangeListener(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, nullptr);
}

void Scene::removePathfindingAgentMotionChangeListener(const entityx::Entity& entity)
{
	auto pathfindingAgentComponent = entityComponentSystem_.component<ecs::PathfindingAgentComponent>(entity.id());
	
	pathfindingEngine_->setMotionChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, nullptr);
}

void Scene::addPathfindingMovementRequestStateChangeListener(const entityx::Entity& entity)
{
	auto pathfindingAgentComponent = entityComponentSystem_.component<ecs::PathfindingAgentComponent>(entity.id());

	std::unique_ptr<IceEnginePathfindingMovementRequestStateChangeListener> movementRequestStateChangeListener = std::make_unique<IceEnginePathfindingMovementRequestStateChangeListener>(entity, this);

	pathfindingEngine_->setStateChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, std::move(movementRequestStateChangeListener));
}

void Scene::removePathfindingMovementRequestStateChangeListener(const entityx::Entity& entity)
{
	auto pathfindingAgentComponent = entityComponentSystem_.component<ecs::PathfindingAgentComponent>(entity.id());

	pathfindingEngine_->setStateChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, nullptr);
}

void Scene::addUserData(const entityx::Entity& entity, const ecs::RigidBodyObjectComponent& rigidBodyObjectComponent)
{
	physics::UserData userData;
	userData.value(entity);
	physicsEngine_->setUserData(physicsSceneHandle_, rigidBodyObjectComponent.rigidBodyObjectHandle, userData);
}

void Scene::addUserData(const entityx::Entity& entity, const ecs::GhostObjectComponent& ghostObjectComponent)
{
	physics::UserData userData;
	userData.value(entity);
	physicsEngine_->setUserData(physicsSceneHandle_, ghostObjectComponent.ghostObjectHandle, userData);
}

void Scene::addUserData(const entityx::Entity& entity, const ecs::PathfindingAgentComponent& pathfindingAgentComponent)
{
	pathfinding::UserData userData;
	userData.value(entity);
	pathfindingEngine_->setUserData(pathfindingSceneHandle_, pathfindingAgentComponent.crowdHandle, pathfindingAgentComponent.agentHandle, userData);
}

void Scene::removeUserData(const entityx::Entity& entity, const ecs::RigidBodyObjectComponent& rigidBodyObjectComponent)
{
	physicsEngine_->setUserData(physicsSceneHandle_, rigidBodyObjectComponent.rigidBodyObjectHandle, physics::UserData());
}

void Scene::removeUserData(const entityx::Entity& entity, const ecs::GhostObjectComponent& ghostObjectComponent)
{
	physicsEngine_->setUserData(physicsSceneHandle_, ghostObjectComponent.ghostObjectHandle, physics::UserData());
}

void Scene::removeUserData(const entityx::Entity& entity, const ecs::PathfindingAgentComponent& pathfindingAgentComponent)
{
	pathfindingEngine_->setUserData(pathfindingSceneHandle_, pathfindingAgentComponent.crowdHandle, pathfindingAgentComponent.agentHandle, pathfinding::UserData());
}

graphics::RenderableHandle Scene::createRenderable(const ModelHandle& modelHandle, const std::string& name)
{
	return gameEngine_->createRenderable(renderSceneHandle_, modelHandle, name);
}

graphics::RenderableHandle Scene::createRenderable(const graphics::MeshHandle& meshHandle, const graphics::TextureHandle& textureHandle, const std::string& name)
{
	return gameEngine_->createRenderable(renderSceneHandle_, meshHandle, textureHandle, name);
}

void Scene::destroy(const graphics::RenderableHandle& renderableHandle)
{
	graphicsEngine_->destroy(renderSceneHandle_, renderableHandle);
}

audio::SoundSourceHandle Scene::play(const audio::SoundHandle& soundHandle, const glm::vec3& position)
{
	return audioEngine_->play(audioSceneHandle_, soundHandle, position);
}

graphics::PointLightHandle Scene::createPointLight(const glm::vec3& position)
{
	return graphicsEngine_->createPointLight(renderSceneHandle_, position);
}

ITerrain* Scene::testCreateTerrain(HeightMap heightMap, SplatMap splatMap, DisplacementMap displacementMap)
{
	//terrainFactory_->create(renderSceneHandle_);
	auto terrain = terrainFactory_->create(properties_, fileSystem_, logger_, this, heightMap, splatMap, displacementMap, graphicsEngine_, pathfindingEngine_, physicsEngine_, audioEngine_, audioSceneHandle_, renderSceneHandle_, physicsSceneHandle_, pathfindingSceneHandle_);
	auto terrainPtr = terrain.get();
	
	terrain_.push_back( std::move(terrain) );
	
	return terrainPtr;
}

std::string Scene::getName() const
{
	return name_;
}

bool Scene::visible() const
{
	return visible_;
}

const SceneStatistics& Scene::getSceneStatistics() const
{
	return sceneStatistics_;
}

entityx::Entity Scene::createEntity()
{
	entityx::Entity e = entityComponentSystem_.create();
	
	LOG_DEBUG(logger_, "Created entity with id: " + std::to_string(e.id().id()) );
	
	return e;
}

void Scene::destroy(const entityx::Entity& entity)
{
	entityComponentSystem_.destroy(entity.id());
}

uint32 Scene::getNumEntities() const
{
	return entityComponentSystem_.numEntities();
}

Raycast Scene::raycast(const ray::Ray& ray)
{
	Raycast result;
	
	auto physicsRaycast = physicsEngine_->raycast(physicsSceneHandle_, ray);
	
	result.setRay(physicsRaycast.ray());
	result.setHitPointWorld(physicsRaycast.hitPointWorld());
	result.setHitNormalWorld(physicsRaycast.hitNormalWorld());
	
	entityx::Entity entity;
	
	if (physicsRaycast.rigidBodyObjectHandle())
	{
		auto userData = physicsEngine_->getUserData(physicsSceneHandle_, physicsRaycast.rigidBodyObjectHandle());
		entity = userData.value<entityx::Entity>();
	}
	else if (physicsRaycast.ghostObjectHandle())
	{
		auto userData = physicsEngine_->getUserData(physicsSceneHandle_, physicsRaycast.ghostObjectHandle());
		entity = userData.value<entityx::Entity>();
	}
	
	result.setEntity(entity);
	
	return result;
}

void Scene::assign(const entityx::Entity& entity, const ecs::GraphicsComponent& component)
{
	entityComponentSystem_.assign<ecs::GraphicsComponent>(entity.id(), std::forward<const ecs::GraphicsComponent>(component));
}

void ice_engine::Scene::assign(const entityx::Entity& entity, const ecs::ScriptObjectComponent& component)
{
	entityComponentSystem_.assign<ecs::ScriptObjectComponent>(entity.id(), std::forward<const ecs::ScriptObjectComponent>(component));
}

void Scene::assign(const entityx::Entity& entity, const ecs::RigidBodyObjectComponent& component)
{
	entityComponentSystem_.assign<ecs::RigidBodyObjectComponent>(entity.id(), std::forward<const ecs::RigidBodyObjectComponent>(component));
	
	addUserData(entity, component);
	addMotionChangeListener(entity);
}

void Scene::assign(const entityx::Entity& entity, const ecs::GhostObjectComponent& component)
{
	entityComponentSystem_.assign<ecs::GhostObjectComponent>(entity.id(), std::forward<const ecs::GhostObjectComponent>(component));
	
	addUserData(entity, component);
	//addMotionChangeListener(entity);
}

void Scene::assign(const entityx::Entity& entity, const ecs::PathfindingAgentComponent& component)
{
	entityComponentSystem_.assign<ecs::PathfindingAgentComponent>(entity.id(), std::forward<const ecs::PathfindingAgentComponent>(component));
	
	addUserData(entity, component);
	addPathfindingAgentMotionChangeListener(entity);
}

void Scene::assign(const entityx::Entity& entity, const ecs::PositionOrientationComponent& component)
{
	entityComponentSystem_.assign<ecs::PositionOrientationComponent>(entity.id(), std::forward<const ecs::PositionOrientationComponent>(component));
}

void Scene::assign(const entityx::Entity& entity, const ecs::PointLightComponent& component)
{
	entityComponentSystem_.assign<ecs::PointLightComponent>(entity.id(), std::forward<const ecs::PointLightComponent>(component));
}

void Scene::rotate(const entityx::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo)
{
	auto component = entityComponentSystem_.component<ecs::PositionOrientationComponent>(entity.id());
	auto graphicsComponent = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	
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

void Scene::rotate(const entityx::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo)
{
	auto component = entityComponentSystem_.component<ecs::PositionOrientationComponent>(entity.id());
	auto graphicsComponent = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	
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

void Scene::rotation(const entityx::Entity& entity, const float32 degrees, const glm::vec3& axis)
{
	auto component = entityComponentSystem_.component<ecs::PositionOrientationComponent>(entity.id());
	auto graphicsComponent = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	
	component->orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
	
	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, component->orientation);
}

void Scene::rotation(const entityx::Entity& entity, const glm::quat& orientation)
{
	auto component = entityComponentSystem_.component<ecs::PositionOrientationComponent>(entity.id());
	auto graphicsComponent = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	
	component->orientation = glm::normalize( orientation );
	
	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, component->orientation);
}

glm::quat Scene::rotation(const entityx::Entity& entity) const
{
	auto component = entityComponentSystem_.component<const ecs::PositionOrientationComponent>(entity.id());
	return component->orientation;
}

void Scene::translate(const entityx::Entity& entity, const glm::vec3& translate)
{
	auto component = entityComponentSystem_.component<ecs::PositionOrientationComponent>(entity.id());
	auto graphicsComponent = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	auto pointLightComponent = entityComponentSystem_.component<ecs::PointLightComponent>(entity.id());
	
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

void Scene::scale(const entityx::Entity& entity, const float32 scale)
{
	auto component = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	component->scale = glm::vec3(scale, scale, scale);
	
	graphicsEngine_->scale(renderSceneHandle_, component->renderableHandle, scale);
}

void Scene::scale(const entityx::Entity& entity, const glm::vec3& scale)
{
	auto component = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	component->scale = scale;
	
	graphicsEngine_->scale(renderSceneHandle_, component->renderableHandle, scale);
}

void Scene::scale(const entityx::Entity& entity, const float32 x, const float32 y, const float32 z)
{
	auto component = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	component->scale = glm::vec3(x, y, z);
	
	graphicsEngine_->scale(renderSceneHandle_, component->renderableHandle, x, y, z);
}

glm::vec3 Scene::scale(const entityx::Entity& entity) const
{
	auto component = entityComponentSystem_.component<const ecs::GraphicsComponent>(entity.id());
	return component->scale;
}

void Scene::lookAt(const entityx::Entity& entity, const glm::vec3& lookAt)
{
	auto component = entityComponentSystem_.component<ecs::PositionOrientationComponent>(entity.id());
	auto graphicsComponent = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	
	const glm::mat4 lookAtMatrix = glm::lookAt(component->position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	component->orientation =  glm::normalize( component->orientation * glm::quat_cast( lookAtMatrix ) );
	
	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, component->orientation);
}

void Scene::position(const entityx::Entity& entity, const glm::vec3& position)
{
	auto component = entityComponentSystem_.component<ecs::PositionOrientationComponent>(entity.id());
	auto graphicsComponent = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	auto ghostObjectComponent = entityComponentSystem_.component<ecs::GhostObjectComponent>(entity.id());
	
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

void Scene::position(const entityx::Entity& entity, const float32 x, const float32 y, const float32 z)
{
	auto component = entityComponentSystem_.component<ecs::PositionOrientationComponent>(entity.id());
	auto graphicsComponent = entityComponentSystem_.component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_.component<ecs::RigidBodyObjectComponent>(entity.id());
	
	component->position = glm::vec3(x, y, z);
	
	graphicsEngine_->position(renderSceneHandle_, graphicsComponent->renderableHandle, x, y, z);
	physicsEngine_->position(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, x, y, z);
}

glm::vec3 Scene::position(const entityx::Entity& entity) const
{
	auto component = entityComponentSystem_.component<const ecs::PositionOrientationComponent>(entity.id());
	return component->position;
}

}
