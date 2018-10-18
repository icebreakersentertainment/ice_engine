#include <chrono>
#include <fstream>
#include <sstream>

#include "exceptions/Throw.hpp"
#include <boost/archive/text_oarchive.hpp>

#include <glm/gtx/string_cast.hpp>

#include "Scene.hpp"

#include "ecs/EntityComponentSystem.hpp"
#include "EntityComponentSystemEventListener.hpp"

#include "IceEngineMotionChangeListener.hpp"
#include "IceEnginePathfindingAgentMotionChangeListener.hpp"
#include "IceEnginePathfindingAgentStateChangeListener.hpp"
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
		openGlLoader_(openGlLoader),
		entityComponentSystem_(std::make_unique<ecs::EntityComponentSystem>(this))
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
	
	auto filename = std::string("../data/scripts/all.as");
	if (!fileSystem_->exists(filename))
	{
		throw std::runtime_error("Script file '" + filename + "' does not exist.");
	}

	auto source = fileSystem_->readAll(filename, fs::FileFlags::READ);

	moduleHandle_ = scriptingEngine_->createModule(name_, {source});

	entityComponentSystemEventListener_ = std::make_unique<EntityComponentSystemEventListener>(*this, *entityComponentSystem_);
}

void Scene::destroy()
{
	LOG_DEBUG(logger_, "Destroying scene: %s", name_);

	audioEngine_->destroyAudioScene(audioSceneHandle_);
	graphicsEngine_->destroyRenderScene(renderSceneHandle_);
	physicsEngine_->destroyPhysicsScene(physicsSceneHandle_);
	pathfindingEngine_->destroyPathfindingScene(pathfindingSceneHandle_);
	scriptingEngine_->destroyExecutionContext(executionContextHandle_);
	
	if (scriptObjectHandle_)
	{
		scriptingEngine_->releaseScriptObject(scriptObjectHandle_);
	}

	for (auto  entity : entityComponentSystem_->entitiesWithComponents<ecs::ScriptObjectComponent>())
	{
		auto handle = entity.component<ecs::ScriptObjectComponent>();

		if (handle->scriptObjectHandle)
		{
			scriptingEngine_->releaseScriptObject(handle->scriptObjectHandle);
		}
	}

	scriptingEngine_->destroyModule(moduleHandle_);
}

void Scene::applyChangesToEntities()
{
	std::vector<ecs::Entity> dirtyEntities;
	for (auto entity : entityComponentSystem_->entitiesWithComponents<ecs::DirtyComponent>())
	{
		dirtyEntities.push_back(entity);

		auto dirtyComponent = entity.component<ecs::DirtyComponent>();

		if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_SOURCE_SCRIPT)
		{
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_POSITION)
			{
				auto pc = entity.component<ecs::PositionComponent>();

				if (auto graphicsComponent = entity.component<ecs::GraphicsComponent>())
				{
					graphicsEngine_->position(renderSceneHandle_, graphicsComponent->renderableHandle, pc->position);
				}
				if (auto rigidBodyObjectComponent = entity.component<ecs::RigidBodyObjectComponent>())
				{
					physicsEngine_->position(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, pc->position);
				}
				if (auto ghostObjectComponent = entity.component<ecs::GhostObjectComponent>())
				{
					physicsEngine_->position(physicsSceneHandle_, ghostObjectComponent->ghostObjectHandle, pc->position);
				}
			}
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_ORIENTATION)
			{
				auto oc = entity.component<ecs::OrientationComponent>();

				if (auto graphicsComponent = entity.component<ecs::GraphicsComponent>())
				{
					graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, oc->orientation);
				}
				if (auto rigidBodyObjectComponent = entity.component<ecs::RigidBodyObjectComponent>())
				{
					physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, oc->orientation);
				}
				if (auto ghostObjectComponent = entity.component<ecs::GhostObjectComponent>())
				{
					physicsEngine_->rotation(physicsSceneHandle_, ghostObjectComponent->ghostObjectHandle, oc->orientation);
				}
			}

			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_RIGID_BODY_OBJECT)
			{
				if (auto rigidBodyObjectComponent = entity.component<ecs::RigidBodyObjectComponent>())
				{
					addUserData(entity, *rigidBodyObjectComponent);
					addMotionChangeListener(entity);
				}
			}
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_GHOST_OBJECT)
			{
				if (auto ghostObjectComponent = entity.component<ecs::GhostObjectComponent>())
				{
					//if (ghostObjectComponent->ghostObjectHandle) addUserData(entity, *ghostObjectComponent);
					addUserData(entity, *ghostObjectComponent);
				}
			}
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_PATHFINDING_AGENT)
			{
				if (auto pathfindingAgentComponent = entity.component<ecs::PathfindingAgentComponent>())
				{
					addUserData(entity, *pathfindingAgentComponent);
					//addPathfindingAgentMotionChangeListener(entity);

					auto motionChangeListener = std::make_unique<IceEnginePathfindingAgentMotionChangeListener>(entity, this);

					pathfindingEngine_->setMotionChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, std::move(motionChangeListener));

					auto stateChangeListener = std::make_unique<IceEnginePathfindingAgentStateChangeListener>(entity);

					pathfindingEngine_->setStateChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, std::move(stateChangeListener));

					auto movementRequestChangeListener = std::make_unique<IceEnginePathfindingMovementRequestStateChangeListener>(entity);

					pathfindingEngine_->setMovementRequestChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, std::move(movementRequestChangeListener));

					if (pathfindingAgentComponent->agentState == pathfinding::MovementRequestState::REQUESTING)
					{
						pathfindingEngine_->requestMoveTarget(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, pathfindingAgentComponent->target);
					}
				}
			}
		}

		if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_SOURCE_PHYSICS)
		{
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_POSITION)
			{
				auto pc = entity.component<ecs::PositionComponent>();

				if (auto graphicsComponent = entity.component<ecs::GraphicsComponent>())
				{
					graphicsEngine_->position(renderSceneHandle_, graphicsComponent->renderableHandle, pc->position);
				}
			}
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_ORIENTATION)
			{
				auto oc = entity.component<ecs::OrientationComponent>();

				if (auto graphicsComponent = entity.component<ecs::GraphicsComponent>())
				{
					graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, oc->orientation);
				}
			}
		}

		if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_SOURCE_PATHFINDING)
		{
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_POSITION)
			{
				auto pc = entity.component<ecs::PositionComponent>();

				if (auto graphicsComponent = entity.component<ecs::GraphicsComponent>())
				{
					graphicsEngine_->position(renderSceneHandle_, graphicsComponent->renderableHandle, pc->position);
				}
				if (auto ghostObjectComponent = entity.component<ecs::GhostObjectComponent>())
				{
					physicsEngine_->position(physicsSceneHandle_, ghostObjectComponent->ghostObjectHandle, pc->position);
				}
			}
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_ORIENTATION)
			{
				auto oc = entity.component<ecs::OrientationComponent>();

				if (auto graphicsComponent = entity.component<ecs::GraphicsComponent>())
				{
					graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, oc->orientation);
				}
				if (auto ghostObjectComponent = entity.component<ecs::GhostObjectComponent>())
				{
					physicsEngine_->rotation(physicsSceneHandle_, ghostObjectComponent->ghostObjectHandle, oc->orientation);
				}
			}
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_AGENT_STATE)
			{
				auto scriptObjectComponent = entity.component<ecs::ScriptObjectComponent>();

				if (scriptObjectComponent && scriptObjectComponent->scriptObjectHandle)
				{
					auto pac = entity.component<ecs::PathfindingAgentComponent>();

					scripting::ParameterList params;
					params.addRef(pac->agentState);

					scriptingEngine_->execute(scriptObjectComponent->scriptObjectHandle, std::string("void update(const AgentState& in)"), params, executionContextHandle_);
				}
			}
			if (dirtyComponent->dirty & ecs::DirtyFlags::DIRTY_MOVEMENT_REQUEST_STATE)
			{
				auto scriptObjectComponent = entity.component<ecs::ScriptObjectComponent>();

				if (scriptObjectComponent && scriptObjectComponent->scriptObjectHandle)
				{
					auto pac = entity.component<ecs::PathfindingAgentComponent>();

					scripting::ParameterList params;
					params.addRef(pac->movementRequestState);

					scriptingEngine_->execute(scriptObjectComponent->scriptObjectHandle, std::string("void update(const MovementRequestState& in)"), params, executionContextHandle_);
				}
			}
		}
	}

	for (auto& entity : dirtyEntities)
	{
		entity.remove<ecs::DirtyComponent>();
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
	
		for (auto e : entityComponentSystem_->entitiesWithComponents<ecs::ScriptObjectComponent>())
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

	for (auto e : entityComponentSystem_->entitiesWithComponents<ecs::GraphicsComponent, ecs::AnimationComponent>())
	{
		auto graphicsComponent = e.component<ecs::GraphicsComponent>();
		auto skeletonComponent = e.component<ecs::SkeletonComponent>();
		auto animationComponent = e.component<ecs::AnimationComponent>();

		if (graphicsComponent->renderableHandle && animationComponent->animationHandle)
		{
			auto& transformations = animationComponent->transformations;
//			gameEngine_->getForegroundThreadPool()->postWork([=, &transformations = animationComponent->transformations]() {
				gameEngine_->animateSkeleton(transformations, graphicsComponent->meshHandle, animationComponent->animationHandle, skeletonComponent->skeletonHandle);
				gameEngine_->getForegroundGraphicsThreadPool()->postWork([=]() {
					graphicsEngine_->update(renderSceneHandle_, graphicsComponent->renderableHandle, animationComponent->bonesHandle, animationComponent->transformations);
				});
//			});
		}
	}

	for (auto e : entityComponentSystem_->entitiesWithComponents<ecs::ParentComponent>())
	{
		auto parentComponent = e.component<ecs::ParentComponent>();

		auto parentPositionComponent = parentComponent->entity.component<ecs::PositionComponent>();
		auto parentOrientationComponent = parentComponent->entity.component<ecs::OrientationComponent>();

		auto positionComponent = e.component<ecs::PositionComponent>();
		auto orientationComponent = e.component<ecs::OrientationComponent>();
		auto graphicsComponent = e.component<ecs::GraphicsComponent>();

		if (graphicsComponent->renderableHandle)
		{
			positionComponent->position = parentPositionComponent->position;
			orientationComponent->orientation = parentOrientationComponent->orientation;

			if (e.hasComponent<ecs::DirtyComponent>())
			{
				auto dirtyComponent = e.component<ecs::DirtyComponent>();
				dirtyComponent->dirty |= ecs::DirtyFlags::DIRTY_SOURCE_SCRIPT | ecs::DirtyFlags::DIRTY_POSITION | ecs::DirtyFlags::DIRTY_ORIENTATION;
			}
			else
			{
				e.assign<ecs::DirtyComponent>(ecs::DirtyFlags::DIRTY_SOURCE_SCRIPT | ecs::DirtyFlags::DIRTY_POSITION | ecs::DirtyFlags::DIRTY_ORIENTATION);
			}
		}
	}

	applyChangesToEntities();
}

void Scene::render()
{
	if (visible()) graphicsEngine_->render(renderSceneHandle_);
}

void Scene::setSceneThingyInstance(void* object)
{
	scriptObjectHandle_ = scripting::ScriptObjectHandle(object);
}

void Scene::setDebugRendering(const bool enabled)
{
	debugRendering_ = enabled;
	
	physicsEngine_->setDebugRendering(physicsSceneHandle_, debugRendering_);
	pathfindingEngine_->setDebugRendering(pathfindingSceneHandle_, debugRendering_);
}

void Scene::createResources(const ecs::Entity& entity)
{
//	auto positionOrientationComponent = entityComponentSystem_->component<ecs::PositionOrientationComponent>(entity.id());
//	auto graphicsComponent = entityComponentSystem_->component<ecs::GraphicsComponent>(entity.id());
//	auto rigidBodyObjectComponent = entityComponentSystem_->component<ecs::RigidBodyObjectComponent>(entity.id());
//	auto pointLightComponent = entityComponentSystem_->component<ecs::PointLightComponent>(entity.id());
//
//	if (positionOrientationComponent)
//	{
//		if (graphicsComponent && !graphicsComponent->renderableHandle)
//		{
//			//graphicsComponent->renderableHandle = gameEngine_->createRenderable(renderSceneHandle_, graphicsComponent->modelHandle);
//			//graphicsEngine_->position(renderSceneHandle_, graphicsComponent->renderableHandle, positionOrientationComponent->position);
//		}
//
//		if (rigidBodyObjectComponent)
//		{
//			//physicsEngine_->position(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, positionOrientationComponent->position);
//		}
//
//		if (pointLightComponent)
//		{
//			//pointLightComponent->position += translate;
//			//graphicsEngine_->position(renderSceneHandle_, pointLightComponent->pointLightHandle, pointLightComponent->position);
//		}
//	}
}

void Scene::destroyResources(const ecs::Entity& entity)
{
}

pathfinding::CrowdHandle Scene::createCrowd(const pathfinding::NavigationMeshHandle& navigationMeshHandle, const pathfinding::CrowdConfig& crowdConfig)
{
	return pathfindingEngine_->createCrowd(pathfindingSceneHandle_, navigationMeshHandle, crowdConfig);
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

void Scene::destroy(const physics::GhostObjectHandle& ghostObjectHandle)
{
	return physicsEngine_->destroy(physicsSceneHandle_, ghostObjectHandle);
}

void Scene::addMotionChangeListener(const ecs::Entity& entity)
{
	auto rigidBodyObjectComponent = entityComponentSystem_->component<ecs::RigidBodyObjectComponent>(entity.id());
	
	std::unique_ptr<IceEngineMotionChangeListener> motionChangeListener = std::make_unique<IceEngineMotionChangeListener>(entity, this);
	
	physicsEngine_->setMotionChangeListener(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, std::move(motionChangeListener));
}

void Scene::addPathfindingAgentMotionChangeListener(const ecs::Entity& entity)
{
	auto pathfindingAgentComponent = entityComponentSystem_->component<ecs::PathfindingAgentComponent>(entity.id());
	
	std::unique_ptr<IceEnginePathfindingAgentMotionChangeListener> motionChangeListener = std::make_unique<IceEnginePathfindingAgentMotionChangeListener>(entity, this);
	
	pathfindingEngine_->setMotionChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, std::move(motionChangeListener));
}

void Scene::removeMotionChangeListener(const ecs::Entity& entity)
{
	auto rigidBodyObjectComponent = entityComponentSystem_->component<ecs::RigidBodyObjectComponent>(entity.id());
	
	physicsEngine_->setMotionChangeListener(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, nullptr);
}

void Scene::removePathfindingAgentMotionChangeListener(const ecs::Entity& entity)
{
	auto pathfindingAgentComponent = entityComponentSystem_->component<ecs::PathfindingAgentComponent>(entity.id());
	
	pathfindingEngine_->setMotionChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, nullptr);
}

void Scene::addPathfindingMovementRequestStateChangeListener(const ecs::Entity& entity)
{
	auto pathfindingAgentComponent = entityComponentSystem_->component<ecs::PathfindingAgentComponent>(entity.id());

	std::unique_ptr<IceEnginePathfindingMovementRequestStateChangeListener> movementRequestStateChangeListener = std::make_unique<IceEnginePathfindingMovementRequestStateChangeListener>(entity);

	pathfindingEngine_->setMovementRequestChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, std::move(movementRequestStateChangeListener));
}

void Scene::removePathfindingMovementRequestStateChangeListener(const ecs::Entity& entity)
{
	auto pathfindingAgentComponent = entityComponentSystem_->component<ecs::PathfindingAgentComponent>(entity.id());

	pathfindingEngine_->setStateChangeListener(pathfindingSceneHandle_, pathfindingAgentComponent->crowdHandle, pathfindingAgentComponent->agentHandle, nullptr);
}

void Scene::addUserData(const ecs::Entity& entity, const ecs::RigidBodyObjectComponent& rigidBodyObjectComponent)
{
	physicsEngine_->setUserData(physicsSceneHandle_, rigidBodyObjectComponent.rigidBodyObjectHandle, boost::any(entity));
}

void Scene::addUserData(const ecs::Entity& entity, const ecs::GhostObjectComponent& ghostObjectComponent)
{
	physicsEngine_->setUserData(physicsSceneHandle_, ghostObjectComponent.ghostObjectHandle, boost::any(entity));
}

void Scene::addUserData(const ecs::Entity& entity, const ecs::PathfindingAgentComponent& pathfindingAgentComponent)
{
	pathfinding::UserData userData;
	userData.value(entity);
	pathfindingEngine_->setUserData(pathfindingSceneHandle_, pathfindingAgentComponent.crowdHandle, pathfindingAgentComponent.agentHandle, userData);
}

void Scene::removeUserData(const ecs::Entity& entity, const ecs::RigidBodyObjectComponent& rigidBodyObjectComponent)
{
	physicsEngine_->setUserData(physicsSceneHandle_, rigidBodyObjectComponent.rigidBodyObjectHandle, boost::any());
}

void Scene::removeUserData(const ecs::Entity& entity, const ecs::GhostObjectComponent& ghostObjectComponent)
{
	physicsEngine_->setUserData(physicsSceneHandle_, ghostObjectComponent.ghostObjectHandle, boost::any());
}

void Scene::removeUserData(const ecs::Entity& entity, const ecs::PathfindingAgentComponent& pathfindingAgentComponent)
{
	pathfindingEngine_->setUserData(pathfindingSceneHandle_, pathfindingAgentComponent.crowdHandle, pathfindingAgentComponent.agentHandle, pathfinding::UserData());
}

graphics::RenderableHandle Scene::createRenderable(
	const ModelHandle& modelHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const glm::vec3& scale
)
{
	return gameEngine_->createRenderable(renderSceneHandle_, modelHandle, position, orientation, scale);
}

graphics::RenderableHandle Scene::createRenderable(
	const graphics::MeshHandle& meshHandle,
	const graphics::TextureHandle& textureHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const glm::vec3& scale
)
{
	return gameEngine_->createRenderable(renderSceneHandle_, meshHandle, textureHandle, position, orientation, scale);
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

void Scene::destroy(const graphics::PointLightHandle& pointLightHandle)
{
	graphicsEngine_->destroy(renderSceneHandle_, pointLightHandle);
}

void Scene::serialize(const std::string& filename)
{
	logger_->info("Serializing scene " + getName() + " to file " + filename);

	auto file = fileSystem_->open(filename, fs::FileFlags::WRITE);

	serialization::TextOutArchive ar(file->getOutputStream());

	ar & *this;
}

void Scene::deserialize(const std::string& filename)
{
	logger_->info("Deserializing scene " + getName() + " from file " + filename);

	auto file = fileSystem_->open(filename, fs::FileFlags::READ);

	serialization::TextInArchive ar(file->getInputStream());

	ar & *this;
}

std::string Scene::getName() const
{
	return name_;
}

void Scene::setVisible(const bool visible)
{
	visible_ = visible;
}

bool Scene::visible() const
{
	return visible_;
}

const SceneStatistics& Scene::getSceneStatistics() const
{
	return sceneStatistics_;
}

ecs::Entity Scene::createEntity()
{
	ecs::Entity e = entityComponentSystem_->create();
	
	LOG_DEBUG(logger_, "Created entity with id: " + std::to_string(e.id().id()) );
	
	return e;
}

void Scene::destroy(ecs::Entity& entity)
{
	entityComponentSystem_->destroy(entity.id());
}

uint32 Scene::getNumEntities() const
{
	return entityComponentSystem_->numEntities();
}

Raycast Scene::raycast(const ray::Ray& ray)
{
	Raycast result;
	
	auto physicsRaycast = physicsEngine_->raycast(physicsSceneHandle_, ray);

	result.setRay(physicsRaycast.ray());
	result.setHitPointWorld(physicsRaycast.hitPointWorld());
	result.setHitNormalWorld(physicsRaycast.hitNormalWorld());
	
	ecs::Entity entity;
	
	if (physicsRaycast.rigidBodyObjectHandle())
	{
		auto userData = physicsEngine_->getUserData(physicsSceneHandle_, physicsRaycast.rigidBodyObjectHandle());
		entity = boost::any_cast<ecs::Entity>(userData);
	}
	else if (physicsRaycast.ghostObjectHandle())
	{
		auto userData = physicsEngine_->getUserData(physicsSceneHandle_, physicsRaycast.ghostObjectHandle());
		entity = boost::any_cast<ecs::Entity>(userData);
	}
	
	result.setEntity(entity);
	
	return result;
}

std::unordered_map<scripting::ScriptObjectHandle, std::string> Scene::getScriptObjectNameMap() const
{
	std::unordered_map<scripting::ScriptObjectHandle, std::string> map;

	for (auto entity : entityComponentSystem_->entitiesWithComponents<ecs::ScriptObjectComponent>())
		{
			auto componentHandle = entity.component<ecs::ScriptObjectComponent>();

			map[componentHandle->scriptObjectHandle] = scriptingEngine_->getScriptObjectName(componentHandle->scriptObjectHandle);
		}

	return map;
}

void normalizeHandles(
	ecs::EntityComponentSystem& entityComponentSystem,
	const std::unordered_map<physics::CollisionShapeHandle, physics::CollisionShapeHandle>& normalizedMap,
	logger::ILogger* logger
)
{
	LOG_DEBUG(logger, "Normalizing CollisionShapeHandles");
//	std::cout << "normalizedMap size " << normalizedMap.size() << std::endl;
//	for (const auto& kv : normalizedMap)
//		{
//			std::cout << "KV " << kv.first << " " << kv.second << std::endl;
//		}

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::RigidBodyObjectComponent>())
	{
		auto componentHandle = entity.component<ecs::RigidBodyObjectComponent>();
		componentHandle->rigidBodyObjectHandle.invalidate();

		auto component = *componentHandle;
//		std::cout << "find in normalized map " << component.collisionShapeHandle << std::endl;
		component.collisionShapeHandle = normalizedMap.at(component.collisionShapeHandle);
		entity.assign<ecs::RigidBodyObjectComponent>(component);
	}

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::GhostObjectComponent>())
	{
		auto componentHandle = entity.component<ecs::GhostObjectComponent>();
		componentHandle->ghostObjectHandle.invalidate();

		auto component = *componentHandle;
		component.collisionShapeHandle = normalizedMap.at(component.collisionShapeHandle);
		entity.assign<ecs::GhostObjectComponent>(component);
	}
}

void normalizeHandles(
	ecs::EntityComponentSystem& entityComponentSystem,
	const std::unordered_map<graphics::MeshHandle, graphics::MeshHandle>& normalizedMeshHandleMap,
	const std::unordered_map<graphics::TextureHandle, graphics::TextureHandle>& normalizedTextureHandleMap,
	logger::ILogger* logger
)
{
	LOG_DEBUG(logger, "Normalizing MeshHandles and TextureHandles");
//	std::cout << "normalizedMeshHandleMap size " << normalizedMeshHandleMap.size() << std::endl;
//	for (const auto& kv : normalizedMeshHandleMap)
//		{
//			std::cout << "KV " << kv.first << " " << kv.second << std::endl;
//		}

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::GraphicsComponent>())
	{
		auto componentHandle = entity.component<ecs::GraphicsComponent>();
		componentHandle->renderableHandle.invalidate();

		auto component = *componentHandle;
//		std::cout << "find in normalized map " << component.meshHandle << std::endl;

		try
		{
			component.meshHandle = normalizedMeshHandleMap.at(component.meshHandle);
		}
		catch (const std::out_of_range& e)
		{
			throw Exception("Unable to find mesh handle " + std::to_string(component.meshHandle.id()));
		}

		try
		{
			component.textureHandle = normalizedTextureHandleMap.at(component.textureHandle);
		}
		catch (const std::out_of_range& e)
		{
			throw Exception("Unable to find texture handle " + std::to_string(component.textureHandle.id()));
		}

		entity.assign<ecs::GraphicsComponent>(component);
	}
}

void normalizeHandles(
	ecs::EntityComponentSystem& entityComponentSystem,
	const std::unordered_map<SkeletonHandle, SkeletonHandle>& normalizedSkeletonHandleMap,
	logger::ILogger* logger
)
{
	LOG_DEBUG(logger, "Normalizing SkeletonHandles");
//	std::cout << "normalizedMap size " << normalizedMap.size() << std::endl;
//	for (const auto& kv : normalizedMap)
//		{
//			std::cout << "KV " << kv.first << " " << kv.second << std::endl;
//		}

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::SkeletonComponent>())
	{
		auto componentHandle = entity.component<ecs::SkeletonComponent>();

		auto component = *componentHandle;
//		std::cout << "find in normalized map " << component.modelHandle << std::endl;
		component.skeletonHandle = normalizedSkeletonHandleMap.at(component.skeletonHandle);
		entity.assign<ecs::SkeletonComponent>(component);
	}
}

void normalizeHandles(
	ecs::EntityComponentSystem& entityComponentSystem,
	const std::unordered_map<AnimationHandle, AnimationHandle>& normalizedAnimationHandleMap,
	logger::ILogger* logger
)
{
	LOG_DEBUG(logger, "Normalizing AnimationHandles");
//	std::cout << "normalizedMap size " << normalizedMap.size() << std::endl;
//	for (const auto& kv : normalizedMap)
//		{
//			std::cout << "KV " << kv.first << " " << kv.second << std::endl;
//		}

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::AnimationComponent>())
	{
		auto componentHandle = entity.component<ecs::AnimationComponent>();
		componentHandle->bonesHandle.invalidate();

		auto component = *componentHandle;
//		std::cout << "find in normalized map " << component.modelHandle << std::endl;
		component.animationHandle = normalizedAnimationHandleMap.at(component.animationHandle);
		entity.assign<ecs::AnimationComponent>(component);
	}
}

void normalizeHandles(
	ecs::EntityComponentSystem& entityComponentSystem,
	const std::unordered_map<graphics::TerrainHandle, graphics::TerrainHandle>& normalizedMap,
	logger::ILogger* logger
)
{
	LOG_DEBUG(logger, "Normalizing TerrainHandles");
//	std::cout << "normalizedMap size " << normalizedMap.size() << std::endl;
//	for (const auto& kv : normalizedMap)
//		{
//			std::cout << "KV " << kv.first << " " << kv.second << std::endl;
//		}

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::GraphicsTerrainComponent>())
	{
		auto componentHandle = entity.component<ecs::GraphicsTerrainComponent>();
		componentHandle->terrainRenderableHandle.invalidate();

		auto component = *componentHandle;
//		std::cout << "find in normalized map " << component.terrainHandle << std::endl;
		component.terrainHandle = normalizedMap.at(component.terrainHandle);
		entity.assign<ecs::GraphicsTerrainComponent>(component);
	}
}

void normalizeHandles(
	ecs::EntityComponentSystem& entityComponentSystem,
	const std::unordered_map<pathfinding::CrowdHandle, pathfinding::CrowdHandle>& normalizedMap,
	logger::ILogger* logger
)
{
	LOG_DEBUG(logger, "Normalizing CrowdHandles");

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::PathfindingAgentComponent>())
	{
		auto componentHandle = entity.component<ecs::PathfindingAgentComponent>();

		componentHandle->agentHandle.invalidate();
		auto component = *componentHandle;

		component.crowdHandle = normalizedMap.at(component.crowdHandle);
		entity.assign<ecs::PathfindingAgentComponent>(component);
	}
}

void normalizeHandles(
	ecs::EntityComponentSystem& entityComponentSystem,
	const std::unordered_map<pathfinding::NavigationMeshHandle, pathfinding::NavigationMeshHandle>& normalizedMap,
	std::unordered_map<pathfinding::CrowdHandle, pathfinding::CrowdHandle>& normalizedCrowdHandleMap,
	logger::ILogger* logger
)
{
	LOG_DEBUG(logger, "Normalizing NavigationMashHandles");

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::PathfindingCrowdComponent>())
	{
		auto componentHandle = entity.component<ecs::PathfindingCrowdComponent>();

		auto oldCrowdHandle = componentHandle->crowdHandle;
		componentHandle->crowdHandle.invalidate();

		auto component = *entity.component<ecs::PathfindingCrowdComponent>();
		component.navigationMeshHandle = normalizedMap.at(component.navigationMeshHandle);
		entity.assign<ecs::PathfindingCrowdComponent>(component);

		auto newCrowdHandle = entity.component<ecs::PathfindingCrowdComponent>()->crowdHandle;

		normalizedCrowdHandleMap[oldCrowdHandle] = newCrowdHandle;
		// update all of the pathfinding agents with the new crowd
//		updateHandles(entityComponentSystem, oldCrowdHandle, newCrowdHandle, logger);
	}
}

void normalizeParentEntities(ecs::EntityComponentSystem& entityComponentSystem,	logger::ILogger* logger)
{
	LOG_DEBUG(logger, "Normalizing parent entities");

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::ParentComponent>())
	{
		auto componentHandle = entity.component<ecs::ParentComponent>();
		auto component = *componentHandle;

		component.entity = entityComponentSystem.get(entityComponentSystem.createId(component.entity.id().index()));
		entity.assign<ecs::ParentComponent>(component);
	}
}

void normalizeChildEntities(ecs::EntityComponentSystem& entityComponentSystem,	logger::ILogger* logger)
{
	LOG_DEBUG(logger, "Normalizing child entities");

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::ChildrenComponent>())
	{
		auto componentHandle = entity.component<ecs::ChildrenComponent>();
		auto component = *componentHandle;

		for (auto& e : component.children)
		{
			e = entityComponentSystem.get(entityComponentSystem.createId(e.id().index()));
		}

		entity.assign<ecs::ChildrenComponent>(component);
	}
}

void normalizeHandles(
	ecs::EntityComponentSystem& entityComponentSystem,
	scripting::IScriptingEngine* scriptingEngine,
	scripting::ModuleHandle moduleHandle,
	scripting::ExecutionContextHandle executionContextHandle,
	const std::unordered_map<scripting::ScriptObjectHandle, std::string>& scriptObjectHandleMap,
	logger::ILogger* logger
)
{
	LOG_DEBUG(logger, "Re-creating script objects");

//	std::cout << "scriptObjectHandleMap size " << scriptObjectHandleMap.size() << std::endl;
//	for (const auto& kv : scriptObjectHandleMap)
//		{
//			std::cout << "KV " << kv.first.get() << " " << kv.second << std::endl;
//		}

	for (auto entity : entityComponentSystem.entitiesWithComponents<ecs::ScriptObjectComponent>())
	{
		auto componentHandle = entity.component<ecs::ScriptObjectComponent>();
//		std::cout << "find in normalized map " << componentHandle->scriptObjectHandle.get() << std::endl;

		auto scriptObjectName = scriptObjectHandleMap.at(componentHandle->scriptObjectHandle);

		LOG_DEBUG(logger, "Re-creating script object");
		componentHandle->scriptObjectHandle = scriptingEngine->createUninitializedScriptObject(moduleHandle, scriptObjectName);

		scripting::ParameterList params;
		params.add(entity);

		scriptingEngine->execute(componentHandle->scriptObjectHandle, std::string("void deserialize(Entity)"), params, executionContextHandle);
	}
}

void Scene::normalizeHandles(
	const std::unordered_map<physics::CollisionShapeHandle, physics::CollisionShapeHandle>& normalizedCollisionShapeHandleMap,
	const std::unordered_map<ModelHandle, ModelHandle>& normalizedModelHandleMap,
	const std::unordered_map<graphics::MeshHandle, graphics::MeshHandle>& normalizedMeshHandleMap,
	const std::unordered_map<graphics::TextureHandle, graphics::TextureHandle>& normalizedTextureHandleMap,
	const std::unordered_map<SkeletonHandle, SkeletonHandle>& normalizedSkeletonHandleMap,
	const std::unordered_map<AnimationHandle, AnimationHandle>& normalizedAnimationHandleMap,
	const std::unordered_map<graphics::TerrainHandle, graphics::TerrainHandle>& normalizedTerrainHandleMap,
	const std::unordered_map<std::string, pathfinding::PolygonMeshHandle>& polygonMeshHandleMap,
	const std::unordered_map<pathfinding::NavigationMeshHandle, pathfinding::NavigationMeshHandle>& normalizedNavigationMeshHandleMap,
	const std::unordered_map<scripting::ScriptObjectHandle, std::string>& scriptObjectHandleMap,
	std::unordered_map<pathfinding::CrowdHandle, pathfinding::CrowdHandle>& normalizedCrowdHandleMap
	)
{
	LOG_DEBUG(logger_, "Normalizing handles");

	ice_engine::normalizeHandles(*entityComponentSystem_, normalizedCollisionShapeHandleMap, logger_);
//	ice_engine::normalizeHandles(*entityComponentSystem_, normalizedModelHandleMap, logger_);
	ice_engine::normalizeHandles(*entityComponentSystem_, normalizedMeshHandleMap, normalizedTextureHandleMap, logger_);
	ice_engine::normalizeHandles(*entityComponentSystem_, normalizedSkeletonHandleMap, logger_);
	ice_engine::normalizeHandles(*entityComponentSystem_, normalizedAnimationHandleMap, logger_);
	ice_engine::normalizeHandles(*entityComponentSystem_, normalizedTerrainHandleMap, logger_);
	ice_engine::normalizeHandles(*entityComponentSystem_, normalizedNavigationMeshHandleMap, normalizedCrowdHandleMap, logger_);
	ice_engine::normalizeParentEntities(*entityComponentSystem_, logger_);
	ice_engine::normalizeChildEntities(*entityComponentSystem_, logger_);
	ice_engine::normalizeHandles(*entityComponentSystem_, scriptingEngine_, moduleHandle_, executionContextHandle_, scriptObjectHandleMap, logger_);
	ice_engine::normalizeHandles(*entityComponentSystem_, normalizedCrowdHandleMap, logger_);
}

/*
void Scene::rotate(ecs::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo)
{
	auto oc = entity.component<ecs::OrientationComponent>();
	auto graphicsComponent = entityComponentSystem_->component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_->component<ecs::RigidBodyObjectComponent>(entity.id());

	switch( relativeTo )
	{
		case graphics::TransformSpace::TS_LOCAL:
			oc->orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) ) * oc->orientation;
			break;

		case graphics::TransformSpace::TS_WORLD:
			oc->orientation =  oc->orientation * glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
			break;

		default:
			throw std::runtime_error(std::string("Invalid TransformSpace type."));
	}

	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, oc->orientation);

	if (rigidBodyObjectComponent)
	{
		physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, oc->orientation);
	}
}

void Scene::rotate(ecs::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo)
{
		auto oc = entity.component<ecs::OrientationComponent>();
	auto graphicsComponent = entityComponentSystem_->component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_->component<ecs::RigidBodyObjectComponent>(entity.id());

	switch( relativeTo )
	{
		case graphics::TransformSpace::TS_LOCAL:
			oc->orientation = oc->orientation * glm::normalize( orientation );
			break;

		case graphics::TransformSpace::TS_WORLD:
			oc->orientation =  glm::normalize( orientation ) * oc->orientation;
			break;

		default:
			throw std::runtime_error(std::string("Invalid TransformSpace type."));
	}

	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, oc->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, oc->orientation);
}

void Scene::rotation(ecs::Entity& entity, const float32 degrees, const glm::vec3& axis)
{
		auto oc = entity.component<ecs::OrientationComponent>();
	auto graphicsComponent = entityComponentSystem_->component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_->component<ecs::RigidBodyObjectComponent>(entity.id());

	oc->orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );

	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, oc->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, oc->orientation);
}

void Scene::rotation(ecs::Entity& entity, const glm::quat& orientation)
{
		auto oc = entity.component<ecs::OrientationComponent>();
	auto graphicsComponent = entityComponentSystem_->component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_->component<ecs::RigidBodyObjectComponent>(entity.id());

	oc->orientation = glm::normalize( orientation );

	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, oc->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, oc->orientation);
}

void Scene::lookAt(ecs::Entity& entity, const glm::vec3& lookAt)
{
	auto pc = entity.component<ecs::PositionComponent>();
		auto oc = entity.component<ecs::OrientationComponent>();
	auto graphicsComponent = entityComponentSystem_->component<ecs::GraphicsComponent>(entity.id());
	auto rigidBodyObjectComponent = entityComponentSystem_->component<ecs::RigidBodyObjectComponent>(entity.id());

	const glm::mat4 lookAtMatrix = glm::lookAt(pc->position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	oc->orientation =  glm::normalize( oc->orientation * glm::quat_cast( lookAtMatrix ) );

	graphicsEngine_->rotation(renderSceneHandle_, graphicsComponent->renderableHandle, oc->orientation);
	physicsEngine_->rotation(physicsSceneHandle_, rigidBodyObjectComponent->rigidBodyObjectHandle, oc->orientation);
}
*/

}
