#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "SceneStatistics.hpp"

#include "ModelHandle.hpp"

#include "Raycast.hpp"

#include "ecs/EntityComponentSystem.hpp"
#include "ecs/GraphicsComponent.hpp"
#include "ecs/ScriptObjectComponent.hpp"
#include "ecs/RigidBodyObjectComponent.hpp"
#include "ecs/GhostObjectComponent.hpp"
#include "ecs/PathfindingAgentComponent.hpp"
#include "ecs/PositionOrientationComponent.hpp"
#include "ecs/PointLightComponent.hpp"

#include "scripting/ScriptObjectHandle.hpp"

#include "audio/SoundHandle.hpp"
#include "audio/SoundSourceHandle.hpp"
#include "graphics/TransformSpace.hpp"
#include "graphics/MeshHandle.hpp"
#include "graphics/TextureHandle.hpp"
#include "graphics/ShaderProgramHandle.hpp"
#include "graphics/PointLightHandle.hpp"
#include "physics/CollisionShapeHandle.hpp"
#include "physics/RigidBodyObjectHandle.hpp"
#include "physics/GhostObjectHandle.hpp"

#include "graphics/IGraphicsEngine.hpp"
#include "ITerrain.hpp"
#include "physics/IPhysicsEngine.hpp"
#include "pathfinding/IPathfindingEngine.hpp"
#include "audio/IAudioEngine.hpp"
#include "scripting/IScriptingEngine.hpp"

#include "GameEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"
#include "IThreadPool.hpp"
#include "IOpenGlLoader.hpp"

namespace ice_engine
{

//class GameEngine;
class EntityComponentSystemEventListener;

class Scene
{
public:
	Scene(
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
	);
	~Scene();

	void tick(const float32 delta);
	void render();
	
	void setSceneThingyInstance(scripting::ScriptObjectHandle scriptObjectHandle);
	
	void setDebugRendering(const bool enabled);
	
	void createResources(const entityx::Entity& entity);
	void destroyResources(const entityx::Entity& entity);

	pathfinding::CrowdHandle createCrowd(const pathfinding::NavigationMeshHandle& navigationMeshHandle);
	void destroy(const pathfinding::CrowdHandle& crowdHandle);
	
	pathfinding::AgentHandle createAgent(const pathfinding::CrowdHandle& crowdHandle, const glm::vec3& position, const pathfinding::AgentParams& agentParams = pathfinding::AgentParams());
	void destroy(const pathfinding::CrowdHandle& crowdHandle, const pathfinding::AgentHandle& agentHandle);
	
	void requestMoveTarget(
		const pathfinding::CrowdHandle& crowdHandle,
		const pathfinding::AgentHandle& agentHandle,
		const glm::vec3& position
	);
	
	void requestMoveVelocity(
		const pathfinding::CrowdHandle& crowdHandle,
		const pathfinding::AgentHandle& agentHandle,
		const glm::vec3& velocity
	);
	
	physics::CollisionShapeHandle createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant);
	physics::CollisionShapeHandle createStaticBoxShape(const glm::vec3& dimensions);
	void destroyStaticShape(const physics::CollisionShapeHandle& collisionShapeHandle);
	void destroyAllStaticShapes();
	
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
	physics::GhostObjectHandle createGhostObject(
		const physics::CollisionShapeHandle& collisionShapeHandle
	);
	physics::GhostObjectHandle createGhostObject(
		const physics::CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation
	);
	
	graphics::RenderableHandle createRenderable(const ModelHandle& modelHandle, const std::string& name = std::string());
	graphics::RenderableHandle createRenderable(const graphics::MeshHandle& meshHandle, const graphics::TextureHandle& textureHandle, const std::string& name = std::string());
	void destroy(const graphics::RenderableHandle& renderableHandle);
	
	audio::SoundSourceHandle play(const audio::SoundHandle& soundHandle, const glm::vec3& position);
	
	graphics::PointLightHandle createPointLight(const glm::vec3& position);
	
	ITerrain* testCreateTerrain(HeightMap heightMap, SplatMap splatMap, DisplacementMap displacementMap);
	
	std::string getName() const;
	bool visible() const;
	
	const SceneStatistics& getSceneStatistics() const;
	
	entityx::Entity createEntity();
	void destroy(const entityx::Entity& entity);
	uint32 getNumEntities() const;

	Raycast raycast(const ray::Ray& ray);
	
	void assign(const entityx::Entity& entity, const ecs::GraphicsComponent& component);
	void assign(const entityx::Entity& entity, const ecs::ScriptObjectComponent& component);
	void assign(const entityx::Entity& entity, const ecs::RigidBodyObjectComponent& component);
	void assign(const entityx::Entity& entity, const ecs::GhostObjectComponent& component);
	void assign(const entityx::Entity& entity, const ecs::PathfindingAgentComponent& component);
	void assign(const entityx::Entity& entity, const ecs::PositionOrientationComponent& component);
	void assign(const entityx::Entity& entity, const ecs::PointLightComponent& component);
	
	void rotate(const entityx::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL);
	void rotate(const entityx::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL);
	void rotation(const entityx::Entity& entity, const float32 degrees, const glm::vec3& axis);
	void rotation(const entityx::Entity& entity, const glm::quat& orientation);
	glm::quat rotation(const entityx::Entity& entity) const;
	void translate(const entityx::Entity& entity, const glm::vec3& translate);
	void scale(const entityx::Entity& entity, const float32 scale);
	void scale(const entityx::Entity& entity, const glm::vec3& scale);
	void scale(const entityx::Entity& entity, const float32 x, const float32 y, const float32 z);
	glm::vec3 scale(const entityx::Entity& entity) const;
	void lookAt(const entityx::Entity& entity, const glm::vec3& lookAt);
	
	void position(const entityx::Entity& entity, const glm::vec3& position);
	void position(const entityx::Entity& entity, const float32 x, const float32 y, const float32 z);
	glm::vec3 position(const entityx::Entity& entity) const;
	
private:
	std::string name_;
	bool visible_ = true;

	GameEngine* gameEngine_;
	audio::IAudioEngine* audioEngine_;
	graphics::IGraphicsEngine* graphicsEngine_;
	ITerrainFactory* terrainFactory_;
	physics::IPhysicsEngine* physicsEngine_;
	pathfinding::IPathfindingEngine* pathfindingEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	IThreadPool* threadPool_;
	IOpenGlLoader* openGlLoader_;
	
	bool debugRendering_ = false;
	
	audio::AudioSceneHandle audioSceneHandle_;
	graphics::RenderSceneHandle renderSceneHandle_;
	physics::PhysicsSceneHandle physicsSceneHandle_;
	pathfinding::PathfindingSceneHandle pathfindingSceneHandle_;
	scripting::ExecutionContextHandle executionContextHandle_;
	
	scripting::ScriptObjectHandle scriptObjectHandle_;
	
	SceneStatistics sceneStatistics_;
	
	// Entity system
	ecs::EntityComponentSystem entityComponentSystem_;
	std::unique_ptr<EntityComponentSystemEventListener> entityComponentSystemEventListener_;
	std::vector<entityx::Entity> entities_;
	
	std::vector<std::unique_ptr<ITerrain>> terrain_;
	
	void initialize();
	void destroy();
	
	void addMotionChangeListener(const entityx::Entity& entity);
	void addPathfindingAgentMotionChangeListener(const entityx::Entity& entity);
	void addPathfindingMovementRequestStateChangeListener(const entityx::Entity& entity);
	void removeMotionChangeListener(const entityx::Entity& entity);
	void removePathfindingAgentMotionChangeListener(const entityx::Entity& entity);
	void removePathfindingMovementRequestStateChangeListener(const entityx::Entity& entity);
	
	void addUserData(const entityx::Entity& entity, const ecs::RigidBodyObjectComponent& rigidBodyObjectComponent);
	void addUserData(const entityx::Entity& entity, const ecs::GhostObjectComponent& ghostObjectComponent);
	void addUserData(const entityx::Entity& entity, const ecs::PathfindingAgentComponent& pathfindingAgentComponent);
	void removeUserData(const entityx::Entity& entity, const ecs::RigidBodyObjectComponent& rigidBodyObjectComponent);
	void removeUserData(const entityx::Entity& entity, const ecs::GhostObjectComponent& ghostObjectComponent);
	void removeUserData(const entityx::Entity& entity, const ecs::PathfindingAgentComponent& pathfindingAgentComponent);
};

}

#endif /* SCENE_H_ */
