#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <string>
#include <unordered_map>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

#include "SceneStatistics.hpp"

#include "ModelHandle.hpp"

#include "Raycast.hpp"

#include "ScriptFunctionHandleWrapper.hpp"

#include "ecs/EntityComponentSystem.hpp"
#include "ecs/Entity.hpp"
#include "ecs/GraphicsComponent.hpp"
#include "ecs/ScriptObjectComponent.hpp"
#include "ecs/RigidBodyObjectComponent.hpp"
#include "ecs/GhostObjectComponent.hpp"
#include "ecs/PathfindingAgentComponent.hpp"
#include "ecs/PositionComponent.hpp"
#include "ecs/OrientationComponent.hpp"
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
#include "serialization/ISerializable.hpp"
#include "serialization/TextOutArchive.hpp"
#include "serialization/TextInArchive.hpp"
#include "serialization/SplitMember.hpp"
#include "serialization/std/Map.hpp"
#include "serialization/std/UnorderedMap.hpp"
#include "IThreadPool.hpp"
#include "IOpenGlLoader.hpp"

namespace ice_engine
{

//class GameEngine;
namespace ecs
{
class EntityComponentSystem;
}

class EntityComponentSystemEventListener;

class Scene : serialization::ISerializable
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
	
	void setSceneThingyInstance(void* object);
	
	void setDebugRendering(const bool enabled);
	
	void createResources(const ecs::Entity& entity);
	void destroyResources(const ecs::Entity& entity);

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
	
	audio::SoundSourceHandle play(const audio::SoundHandle& soundHandle, const glm::vec3& position);
	
	graphics::PointLightHandle createPointLight(const glm::vec3& position);
	void destroy(const graphics::PointLightHandle& pointLightHandle);
	
	
	graphics::TerrainRenderableHandle createTerrainRenderable(const graphics::TerrainHandle terrainHandle)
	{
		return graphicsEngine_->createTerrainRenderable(renderSceneHandle_, terrainHandle);
	}
	
	void destroy(const graphics::TerrainRenderableHandle& terrainRenderableHandle)
	{
		graphicsEngine_->destroy(renderSceneHandle_, terrainRenderableHandle);
	}
	
	ITerrain* testCreateTerrain(HeightMap heightMap, SplatMap splatMap, DisplacementMap displacementMap, physics::CollisionShapeHandle collisionShapeHandle, pathfinding::PolygonMeshHandle polygonMeshHandle,
			pathfinding::NavigationMeshHandle navigationMeshHandle)
	{
		//terrainFactory_->create(renderSceneHandle_);
		auto terrain = terrainFactory_->create(properties_, fileSystem_, logger_, this, heightMap, splatMap, displacementMap, collisionShapeHandle, polygonMeshHandle, navigationMeshHandle, graphicsEngine_, pathfindingEngine_, physicsEngine_, audioEngine_, audioSceneHandle_, renderSceneHandle_, physicsSceneHandle_, pathfindingSceneHandle_);
		auto terrainPtr = terrain.get();

		terrain_.push_back( std::move(terrain) );

		return terrainPtr;
	}
	
	template <typename ... C>
	void entitiesWithComponents(void* object)
	{
		scripting::ScriptFunctionHandle scriptFunctionHandle(object);

		auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_, scriptFunctionHandle);

		for (auto entity : entityComponentSystem_->entitiesWithComponents<C ...>())
		{
			scripting::ParameterList params;
			params.add(entity);

			scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), params, executionContextHandle_);
		}
	}

	template <typename ... C>
	void entitiesWithComponents(void* object) const
	{
		scripting::ScriptFunctionHandle scriptFunctionHandle(object);

		auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_, scriptFunctionHandle);

		for (const auto entity : const_cast<EntityComponentSystem*>(entityComponentSystem_)->entities_.entities_with_components<C ...>())
		{
			scripting::ParameterList params;
			params.add(entity);

			scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), params, executionContextHandle_);
		}
	}

	virtual void serialize(const std::string& filename) override;
	virtual void deserialize(const std::string& filename) override;

	void addPreSerializeCallback(std::function<void(serialization::TextOutArchive&, ecs::EntityComponentSystem&, const unsigned int)> callback)
	{
		preSerializeCallbacks_.push_back(callback);
	}

	void addPostSerializeCallback(std::function<void(serialization::TextOutArchive&, ecs::EntityComponentSystem&, const unsigned int)> callback)
	{
		postSerializeCallbacks_.push_back(callback);
	}

	void addPreDeserializeCallback(std::function<void(serialization::TextInArchive&, ecs::EntityComponentSystem&, const unsigned int)> callback)
	{
		preDeserializeCallbacks_.push_back(callback);
	}

	void addPostDeserializeCallback(
		std::function<
			void(
				serialization::TextInArchive&,
				ecs::EntityComponentSystem&,
				const std::unordered_map<physics::CollisionShapeHandle, physics::CollisionShapeHandle>&,
				const std::unordered_map<ModelHandle, ModelHandle>&,
				const std::unordered_map<graphics::TerrainHandle, graphics::TerrainHandle>&,
				const std::unordered_map<std::string, pathfinding::PolygonMeshHandle>&,
				const std::unordered_map<pathfinding::NavigationMeshHandle, pathfinding::NavigationMeshHandle>&,
				const std::unordered_map<scripting::ScriptObjectHandle, std::string>&,
				const std::unordered_map<pathfinding::CrowdHandle, pathfinding::CrowdHandle>&,
				const unsigned int
			)
		>
		callback
	)
	{
		postDeserializeCallbacks_.push_back(callback);
	}

	void addPreSerializeCallback(void* object)
	{
		scripting::ScriptFunctionHandle scriptFunctionHandle(object);

		auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_, scriptFunctionHandle);

		scriptPreSerializeCallbacks_.push_back(scriptFunctionHandleWrapper);
	}

	void addPostSerializeCallback(void* object)
	{
		scripting::ScriptFunctionHandle scriptFunctionHandle(object);

		auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_, scriptFunctionHandle);

		scriptPostSerializeCallbacks_.push_back(scriptFunctionHandleWrapper);
	}

	void addPreDeserializeCallback(void* object)
	{
		scripting::ScriptFunctionHandle scriptFunctionHandle(object);

		auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_, scriptFunctionHandle);

		scriptPreDeserializeCallbacks_.push_back(scriptFunctionHandleWrapper);
	}

	void addPostDeserializeCallback(void* object)
	{
		scripting::ScriptFunctionHandle scriptFunctionHandle(object);

		auto scriptFunctionHandleWrapper = ScriptFunctionHandleWrapper(scriptingEngine_, scriptFunctionHandle);

		scriptPostDeserializeCallbacks_.push_back(scriptFunctionHandleWrapper);
	}

	std::string getName() const;

	void setVisible(const bool visible);
	bool visible() const;
	
	const SceneStatistics& getSceneStatistics() const;
	
	ecs::Entity createEntity();
	void destroy(ecs::Entity& entity);
	uint32 getNumEntities() const;

	Raycast raycast(const ray::Ray& ray);
	
private:
	friend class boost::serialization::access;

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
	scripting::ModuleHandle moduleHandle_;
	
	scripting::ScriptObjectHandle scriptObjectHandle_;
	
	SceneStatistics sceneStatistics_;
	
	// ecs::Entity system
	std::unique_ptr<ecs::EntityComponentSystem> entityComponentSystem_;
	std::unique_ptr<EntityComponentSystemEventListener> entityComponentSystemEventListener_;
	std::vector<ecs::Entity> entities_;
	
	std::vector<std::unique_ptr<ITerrain>> terrain_;
	
	std::vector<std::function<void(serialization::TextOutArchive&, ecs::EntityComponentSystem&, const unsigned int)>> preSerializeCallbacks_;
	std::vector<std::function<void(serialization::TextOutArchive&, ecs::EntityComponentSystem&, const unsigned int)>> postSerializeCallbacks_;
	std::vector<std::function<void(serialization::TextInArchive&, ecs::EntityComponentSystem&, const unsigned int)>> preDeserializeCallbacks_;
	std::vector<std::function<
		void(
				serialization::TextInArchive&,
				ecs::EntityComponentSystem&,
				const std::unordered_map<physics::CollisionShapeHandle, physics::CollisionShapeHandle>&,
				const std::unordered_map<ModelHandle, ModelHandle>&,
				const std::unordered_map<graphics::TerrainHandle, graphics::TerrainHandle>&,
				const std::unordered_map<std::string, pathfinding::PolygonMeshHandle>&,
				const std::unordered_map<pathfinding::NavigationMeshHandle, pathfinding::NavigationMeshHandle>&,
				const std::unordered_map<scripting::ScriptObjectHandle, std::string>&,
				const std::unordered_map<pathfinding::CrowdHandle, pathfinding::CrowdHandle>&,
				const unsigned int
			)
	>> postDeserializeCallbacks_;

	std::vector<ScriptFunctionHandleWrapper> scriptPreSerializeCallbacks_;
	std::vector<ScriptFunctionHandleWrapper> scriptPostSerializeCallbacks_;
	std::vector<ScriptFunctionHandleWrapper> scriptPreDeserializeCallbacks_;
	std::vector<ScriptFunctionHandleWrapper> scriptPostDeserializeCallbacks_;

	void initialize();
	void destroy();
	
	void applyChangesToEntities();

	void addMotionChangeListener(const ecs::Entity& entity);
	void addPathfindingAgentMotionChangeListener(const ecs::Entity& entity);
	void addPathfindingMovementRequestStateChangeListener(const ecs::Entity& entity);
	void removeMotionChangeListener(const ecs::Entity& entity);
	void removePathfindingAgentMotionChangeListener(const ecs::Entity& entity);
	void removePathfindingMovementRequestStateChangeListener(const ecs::Entity& entity);

	void addUserData(const ecs::Entity& entity, const ecs::RigidBodyObjectComponent& rigidBodyObjectComponent);
	void addUserData(const ecs::Entity& entity, const ecs::GhostObjectComponent& ghostObjectComponent);
	void addUserData(const ecs::Entity& entity, const ecs::PathfindingAgentComponent& pathfindingAgentComponent);
	void removeUserData(const ecs::Entity& entity, const ecs::RigidBodyObjectComponent& rigidBodyObjectComponent);
	void removeUserData(const ecs::Entity& entity, const ecs::GhostObjectComponent& ghostObjectComponent);
	void removeUserData(const ecs::Entity& entity, const ecs::PathfindingAgentComponent& pathfindingAgentComponent);

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		boost::serialization::split_member(ar, *this, version);
	}


	std::unordered_map<scripting::ScriptObjectHandle, std::string> getScriptObjectNameMap() const;

	template<class Archive>
	void saveMappings(Archive& ar, const unsigned int version) const
	{
		ar & gameEngine_->resourceHandleCache().collisionShapeHandleMap();
		ar & gameEngine_->resourceHandleCache().modelHandleMap();
		ar & gameEngine_->resourceHandleCache().terrainHandleMap();
		ar & gameEngine_->resourceHandleCache().polygonMeshHandleMap();
		ar & gameEngine_->resourceHandleCache().navigationMeshHandleMap();
		auto map = getScriptObjectNameMap();
		ar & map;

		std::cout << "SAVE scriptObjectHandleMap size " << map.size() << std::endl;
		for (const auto& kv : map)
			{
				std::cout << "KV " << kv.first.get() << " " << kv.second << std::endl;
			}
	}

	template<class Archive>
	void loadMappings(Archive& ar, const unsigned int version) const
	{
//		loadPhysicsEngineMappings(ar, version);
	}

	template<class Handle>
	auto generateNormalizedMap(
		const std::unordered_map<std::string, Handle>& oldMap,
		const std::unordered_map<std::string, Handle>& newMap,
		logger::ILogger* logger
	)
	{
		std::unordered_map<Handle, Handle> normalizedMap;

		for (const auto& kv : oldMap)
		{
			auto it = newMap.find(kv.first);
			if (it != newMap.end())
			{
				normalizedMap[kv.second] = it->second;
			}
			else
			{
				LOG_WARN(logger, "Could not map " + kv.first + " with id " + std::to_string(kv.second.id()));
			}
		}

		return normalizedMap;
	}

	void normalizeHandles(
		const std::unordered_map<physics::CollisionShapeHandle, physics::CollisionShapeHandle>& normalizedCollisionShapeHandleMap,
		const std::unordered_map<ModelHandle, ModelHandle>& normalizedModelHandleMap,
		const std::unordered_map<graphics::TerrainHandle, graphics::TerrainHandle>& normalizedTerrainHandleMap,
		const std::unordered_map<std::string, pathfinding::PolygonMeshHandle>& polygonMeshHandleMap,
		const std::unordered_map<pathfinding::NavigationMeshHandle, pathfinding::NavigationMeshHandle>& normalizedNavigationMeshHandleMap,
		const std::unordered_map<scripting::ScriptObjectHandle, std::string>& scriptObjectHandleMap,
		std::unordered_map<pathfinding::CrowdHandle, pathfinding::CrowdHandle>& normalizedCrowdHandleMap
	);

	template<class Archive>
	void save(Archive& ar, const unsigned int version) const
	{
		LOG_DEBUG(logger_, "Calling pre serialize callbacks");

		for (auto& scriptFunctionHandleWrapper : scriptPreSerializeCallbacks_)
		{
			scripting::ParameterList params;
			params.addRef(*this);

			scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), params, executionContextHandle_);
		}

		for (auto& callback : preSerializeCallbacks_)
		{
			callback(static_cast<serialization::TextOutArchive&>(ar), *entityComponentSystem_, version);
		}

		LOG_DEBUG(logger_, "Calling serialize on script objects");

		for (auto entity : entityComponentSystem_->entitiesWithComponents<ecs::ScriptObjectComponent>())
		{
			auto componentHandle = entity.component<ecs::ScriptObjectComponent>();

			scripting::ParameterList params;
			params.add(entity);

			scriptingEngine_->execute(componentHandle->scriptObjectHandle, std::string("void serialize(Entity)"), params, executionContextHandle_);
		}

		saveMappings(ar, version);

		ar & *entityComponentSystem_;

		LOG_DEBUG(logger_, "Calling post serialize callbacks");

		for (auto& callback : postSerializeCallbacks_)
		{
			callback(static_cast<serialization::TextOutArchive&>(ar), *entityComponentSystem_, version);
		}

		for (auto& scriptFunctionHandleWrapper : scriptPostSerializeCallbacks_)
		{
			scripting::ParameterList params;
			params.addRef(*this);

			scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), params, executionContextHandle_);
		}
	}

	template<class Archive>
	void load(Archive& ar, const unsigned int version)
	{
		try
		{
		LOG_DEBUG(logger_, "Calling pre deserialize callbacks");

		for (auto& scriptFunctionHandleWrapper : scriptPreDeserializeCallbacks_)
		{
			scripting::ParameterList params;
			params.addRef(*this);

			scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), params, executionContextHandle_);
		}

		for (auto& callback : preDeserializeCallbacks_)
		{
			callback(static_cast<serialization::TextInArchive&>(ar), *entityComponentSystem_, version);
		}

		std::unordered_map<std::string, physics::CollisionShapeHandle> collisionShapeHandleMap;
		ar & collisionShapeHandleMap;

//		std::cout << "collisionShapeHandleMap size " << collisionShapeHandleMap.size() << std::endl;
//		for (const auto& kv : collisionShapeHandleMap)
//		{
//			std::cout << "collisionShapeHandleMap KV " << kv.first << " " << kv.second << std::endl;
//		}

		std::unordered_map<std::string, ModelHandle> modelHandleMap;
		ar & modelHandleMap;

		std::unordered_map<std::string, graphics::TerrainHandle> terrainHandleMap;
		ar & terrainHandleMap;

		std::unordered_map<std::string, pathfinding::PolygonMeshHandle> polygonMeshHandleMap;
		ar & polygonMeshHandleMap;

		std::unordered_map<std::string, pathfinding::NavigationMeshHandle> navigationMeshHandleMap;
		ar & navigationMeshHandleMap;

		std::unordered_map<scripting::ScriptObjectHandle, std::string> scriptObjectHandleMap;
		ar & scriptObjectHandleMap;

		std::unordered_map<pathfinding::CrowdHandle, pathfinding::CrowdHandle> normalizedCrowdHandleMap;

		std::cout << "LOAD scriptObjectHandleMap size " << scriptObjectHandleMap.size() << std::endl;
		for (const auto& kv : scriptObjectHandleMap)
			{
				std::cout << "KV " << kv.first.get() << " " << kv.second << std::endl;
			}

		ar & *entityComponentSystem_;

		auto normalizedCollisionShapeHandleMap = generateNormalizedMap(collisionShapeHandleMap, gameEngine_->resourceHandleCache().collisionShapeHandleMap(), logger_);
		auto normalizedModelHandleMap = generateNormalizedMap(modelHandleMap, gameEngine_->resourceHandleCache().modelHandleMap(), logger_);
		auto normalizedTerrainHandleMap = generateNormalizedMap(terrainHandleMap, gameEngine_->resourceHandleCache().terrainHandleMap(), logger_);
		auto normalizedNavigationMeshHandleMap = generateNormalizedMap(navigationMeshHandleMap, gameEngine_->resourceHandleCache().navigationMeshHandleMap(), logger_);
//		generateNormalizedMap(scriptingEngine_, moduleHandle_, executionContextHandle_, scriptObjectHandleMap, logger_);

		normalizeHandles(normalizedCollisionShapeHandleMap, normalizedModelHandleMap, normalizedTerrainHandleMap, polygonMeshHandleMap, normalizedNavigationMeshHandleMap, scriptObjectHandleMap, normalizedCrowdHandleMap);

		LOG_DEBUG(logger_, "Calling post deserialize callbacks");

		for (auto& callback : postDeserializeCallbacks_)
		{
			callback(static_cast<serialization::TextInArchive&>(ar), *entityComponentSystem_, normalizedCollisionShapeHandleMap, normalizedModelHandleMap, normalizedTerrainHandleMap, polygonMeshHandleMap, normalizedNavigationMeshHandleMap, scriptObjectHandleMap, normalizedCrowdHandleMap, version);
		}

		for (auto& scriptFunctionHandleWrapper : scriptPostDeserializeCallbacks_)
		{
			scripting::ParameterList params;
			params.addRef(*this);

			scriptingEngine_->execute(scriptFunctionHandleWrapper.get(), params, executionContextHandle_);
		}
		}
		 catch(const std::exception& e)
		 {
			 LOG_ERROR(logger_, std::string("Exception: ") + e.what());
			 std::cout << "Exception: " << e.what() << std::endl;
		 }
	}
};

}

#endif /* SCENE_H_ */
