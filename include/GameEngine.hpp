#ifndef GAMEENGINE_H_
#define GAMEENGINE_H_

#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <memory>
#include <future>
#include <string>
#include <mutex>
#include <shared_mutex>
#include <queue>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "EngineStatistics.hpp"
#include "IThreadPool.hpp"
#include "IOpenGlLoader.hpp"
#include "ModelHandle.hpp"
#include "IDebugRenderer.hpp"
//#include "Scene.hpp"
#include "IWindowEventListener.hpp"
#include "IKeyboardEventListener.hpp"
#include "IMouseMotionEventListener.hpp"
#include "IMouseButtonEventListener.hpp"
#include "IMouseWheelEventListener.hpp"
#include "IConnectEventListener.hpp"
#include "IDisconnectEventListener.hpp"
#include "IMessageEventListener.hpp"

#include "graphics/model/Model.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "image/Image.hpp"

#include "extras/FpsCamera.hpp"

#include "utilities/Properties.hpp"
#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "ResourceCache.hpp"
#include "Camera.hpp"
#include "ThreadPool.hpp"
#include "OpenGlLoader.hpp"
#include "DebugRenderer.hpp"
#include "IPluginManager.hpp"

#include "Heightfield.hpp"
#include "PathfindingTerrain.hpp"

#include "graphics/IGraphicsEngineFactory.hpp"
#include "graphics/IGraphicsEngine.hpp"
#include "graphics/IEventListener.hpp"
#include "graphics/gui/IGui.hpp"

#include "ITerrainFactory.hpp"

#include "audio/IAudioEngineFactory.hpp"
#include "audio/IAudioEngine.hpp"

#include "networking/INetworkingEngineFactory.hpp"
#include "networking/INetworkingEngine.hpp"
#include "networking/IEventListener.hpp"

#include "pathfinding/IPathfindingEngineFactory.hpp"
#include "pathfinding/IPathfindingEngine.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "serialization/TextOutArchive.hpp"
#include "serialization/TextInArchive.hpp"

#include "Audio.hpp"

namespace ice_engine
{

class Scene;

namespace ecs
{
class EntityComponentSystem;
}

class ResourceHandleCache
{
public:
	void addCollisionShapeHandle(const std::string& name, const physics::CollisionShapeHandle& handle)
	{
		std::unique_lock<std::shared_mutex> lock(collisionShapeHandleMutex_);

		if (collisionShapeHandleMap_.find(name) != collisionShapeHandleMap_.end())
		{
			throw std::runtime_error("Resource with name already exists.");
		}

		collisionShapeHandleMap_[name] = handle;
	}

	void removeCollisionShapeHandle(const std::string& name)
	{
		std::unique_lock<std::shared_mutex> lock(collisionShapeHandleMutex_);

		auto it = collisionShapeHandleMap_.find(name);
		if (it != collisionShapeHandleMap_.end())
		{
			collisionShapeHandleMap_.erase(it);
		}
	}

	void removeAllCollisionShapeHandles()
	{
		std::unique_lock<std::shared_mutex> lock(collisionShapeHandleMutex_);

		collisionShapeHandleMap_.clear();
	}

	physics::CollisionShapeHandle getCollisionShapeHandle(const std::string& name) const
	{
		std::shared_lock<std::shared_mutex> lock(collisionShapeHandleMutex_);

		auto it = collisionShapeHandleMap_.find(name);
		if (it != collisionShapeHandleMap_.end()) return it->second;

		return physics::CollisionShapeHandle();
	}

	void addModelHandle(const std::string& name, const ModelHandle& handle)
		{
			std::unique_lock<std::shared_mutex> lock(modelHandleMutex_);

			if (modelHandleMap_.find(name) != modelHandleMap_.end())
			{
				throw std::runtime_error("Resource with name already exists.");
			}

			modelHandleMap_[name] = handle;
		}

		void removeModelHandle(const std::string& name)
		{
			std::unique_lock<std::shared_mutex> lock(modelHandleMutex_);

			auto it = modelHandleMap_.find(name);
			if (it != modelHandleMap_.end())
			{
				modelHandleMap_.erase(it);
			}
		}

		void removeAllModelHandles()
		{
			std::unique_lock<std::shared_mutex> lock(modelHandleMutex_);

			modelHandleMap_.clear();
		}

		ModelHandle getModelHandle(const std::string& name) const
		{
			std::shared_lock<std::shared_mutex> lock(modelHandleMutex_);

			auto it = modelHandleMap_.find(name);
			if (it != modelHandleMap_.end()) return it->second;

			return ModelHandle();
		}

	void addTerrainHandle(const std::string& name, const graphics::TerrainHandle& handle)
		{
			std::unique_lock<std::shared_mutex> lock(terrainHandleMutex_);

			if (terrainHandleMap_.find(name) != terrainHandleMap_.end())
			{
				throw std::runtime_error("Resource with name already exists.");
			}

			terrainHandleMap_[name] = handle;
		}

		void removeTerrainHandle(const std::string& name)
		{
			std::unique_lock<std::shared_mutex> lock(terrainHandleMutex_);

			auto it = terrainHandleMap_.find(name);
			if (it != terrainHandleMap_.end())
			{
				terrainHandleMap_.erase(it);
			}
		}

		void removeAllTerrainHandles()
		{
			std::unique_lock<std::shared_mutex> lock(terrainHandleMutex_);

			terrainHandleMap_.clear();
		}

		graphics::TerrainHandle getTerrainHandle(const std::string& name) const
		{
			std::shared_lock<std::shared_mutex> lock(terrainHandleMutex_);

			auto it = terrainHandleMap_.find(name);
			if (it != terrainHandleMap_.end()) return it->second;

			return graphics::TerrainHandle();
		}

	void addPolygonMeshHandle(const std::string& name, const pathfinding::PolygonMeshHandle& handle)
	{
		std::unique_lock<std::shared_mutex> lock(polygonMeshHandleMutex_);

		if (polygonMeshHandleMap_.find(name) != polygonMeshHandleMap_.end())
		{
			throw std::runtime_error("Resource with name already exists.");
		}

		polygonMeshHandleMap_[name] = handle;
	}

	void removePolygonMeshHandle(const std::string& name)
	{
		std::unique_lock<std::shared_mutex> lock(polygonMeshHandleMutex_);

		auto it = polygonMeshHandleMap_.find(name);
		if (it != polygonMeshHandleMap_.end())
		{
			polygonMeshHandleMap_.erase(it);
		}
	}

	void removeAllPolygonMeshHandles()
	{
		std::unique_lock<std::shared_mutex> lock(polygonMeshHandleMutex_);

		polygonMeshHandleMap_.clear();
	}

	pathfinding::PolygonMeshHandle getPolygonMeshHandle(const std::string& name) const
	{
		std::shared_lock<std::shared_mutex> lock(polygonMeshHandleMutex_);

		auto it = polygonMeshHandleMap_.find(name);
		if (it != polygonMeshHandleMap_.end()) return it->second;

		return pathfinding::PolygonMeshHandle();
	}

	void addNavigationMeshHandle(const std::string& name, const pathfinding::NavigationMeshHandle& handle)
	{
		std::unique_lock<std::shared_mutex> lock(navigationMeshHandleMutex_);

		if (navigationMeshHandleMap_.find(name) != navigationMeshHandleMap_.end())
		{
			throw std::runtime_error("Resource with name already exists.");
		}

		navigationMeshHandleMap_[name] = handle;
	}

	void removeNavigationMeshHandle(const std::string& name)
	{
		std::unique_lock<std::shared_mutex> lock(navigationMeshHandleMutex_);

		auto it = navigationMeshHandleMap_.find(name);
		if (it != navigationMeshHandleMap_.end())
		{
			navigationMeshHandleMap_.erase(it);
		}
	}

	void removeAllNavigationMeshHandles()
	{
		std::unique_lock<std::shared_mutex> lock(navigationMeshHandleMutex_);

		navigationMeshHandleMap_.clear();
	}

	pathfinding::NavigationMeshHandle getNavigationMeshHandle(const std::string& name) const
	{
		std::shared_lock<std::shared_mutex> lock(navigationMeshHandleMutex_);

		auto it = navigationMeshHandleMap_.find(name);
		if (it != navigationMeshHandleMap_.end()) return it->second;

		return pathfinding::NavigationMeshHandle();
	}

	std::unordered_map<std::string, physics::CollisionShapeHandle> collisionShapeHandleMap()
		{
		std::shared_lock<std::shared_mutex> lock(collisionShapeHandleMutex_);

			return collisionShapeHandleMap_;
		}

	std::unordered_map<std::string, ModelHandle> modelHandleMap()
		{
		std::shared_lock<std::shared_mutex> lock(modelHandleMutex_);

			return modelHandleMap_;
		}

	std::unordered_map<std::string, graphics::TerrainHandle> terrainHandleMap()
		{
		std::shared_lock<std::shared_mutex> lock(terrainHandleMutex_);

			return terrainHandleMap_;
		}

	std::unordered_map<std::string, pathfinding::PolygonMeshHandle> polygonMeshHandleMap()
		{
		std::shared_lock<std::shared_mutex> lock(polygonMeshHandleMutex_);

		 return polygonMeshHandleMap_;
		}

	std::unordered_map<std::string, pathfinding::NavigationMeshHandle> navigationMeshHandleMap()
		{
			std::shared_lock<std::shared_mutex> lock(navigationMeshHandleMutex_);

			return navigationMeshHandleMap_;
		}

private:
	mutable std::shared_mutex collisionShapeHandleMutex_;
	mutable std::shared_mutex modelHandleMutex_;
	mutable std::shared_mutex terrainHandleMutex_;
	mutable std::shared_mutex polygonMeshHandleMutex_;
	mutable std::shared_mutex navigationMeshHandleMutex_;

	std::unordered_map<std::string, physics::CollisionShapeHandle> collisionShapeHandleMap_;
	std::unordered_map<std::string, ModelHandle> modelHandleMap_;
	std::unordered_map<std::string, graphics::TerrainHandle> terrainHandleMap_;
	std::unordered_map<std::string, pathfinding::PolygonMeshHandle> polygonMeshHandleMap_;
	std::unordered_map<std::string, pathfinding::NavigationMeshHandle> navigationMeshHandleMap_;
};

class GameEngine : public graphics::IEventListener, public networking::IEventListener
{
public:
	GameEngine(
		std::unique_ptr<utilities::Properties> properties,
		std::unique_ptr<ice_engine::IPluginManager> pluginManager,
		std::unique_ptr<ice_engine::logger::ILogger> logger
	);
	GameEngine(
		std::unique_ptr<utilities::Properties> properties,
		std::unique_ptr<ice_engine::logger::ILogger> logger,
		std::unique_ptr<ice_engine::IPluginManager> pluginManager,
		std::unique_ptr<graphics::IGraphicsEngineFactory> graphicsEngineFactory,
		std::unique_ptr<ITerrainFactory> terrainFactory
	);
	virtual ~GameEngine();

	void run();
	
	const EngineStatistics& getEngineStatistics() const;
	
	void setIGameInstance(void* object);

	/**
	 * 
	 */
	void setBootstrapScript(const std::string& filename);
	
	audio::IAudioEngine* getAudioEngine() const;
	graphics::IGraphicsEngine* getGraphicsEngine() const;
	physics::IPhysicsEngine* getPhysicsEngine() const;
	scripting::IScriptingEngine* getScriptingEngine() const;
	IDebugRenderer* getDebugRenderer() const;
	pathfinding::IPathfindingEngine* getPathfindingEngine() const;
	IThreadPool* getBackgroundThreadPool() const;
	IThreadPool* getForegroundThreadPool() const;
	IOpenGlLoader* getOpenGlLoader() const;
	logger::ILogger* getLogger() const;
	fs::IFileSystem* getFileSystem() const;
	ResourceCache& resourceCache()
	{
		return resourceCache_;
	}

	ResourceHandleCache& resourceHandleCache()
	{
		return resourceHandleCache_;
	}
	
	graphics::gui::IGui* createGui(const std::string& name);
	void destroyGui(const graphics::gui::IGui* gui);
	
	void setCallback(graphics::gui::IButton* button, void* object);
	void setCallback(graphics::gui::IMenuItem* menuItem, void* object);
	
	Audio* loadAudio(const std::string& name, const std::string& filename);
	std::shared_future<Audio*> loadAudioAsync(const std::string& name, const std::string& filename);

	template <typename ... Args>
	Image* createImage(const std::string& name, const Args ... args)
	{
		LOG_DEBUG(logger_, "Creating image: " + name);

			resourceCache_.addImage( name, std::make_unique<Image>(args ...) );

			LOG_DEBUG(logger_, "Done creating image: " + name);

			return resourceCache_.getImage(name);
	}

	Image* loadImage(const std::string& name, const std::string& filename);
	std::shared_future<Image*> loadImageAsync(const std::string& name, const std::string& filename);
	graphics::model::Model* loadModel(const std::string& name, const std::string& filename);
	std::shared_future<graphics::model::Model*> loadModelAsync(const std::string& name, const std::string& filename);
	graphics::model::Model* importModel(const std::string& name, const std::string& filename);
	std::shared_future<graphics::model::Model*> importModelAsync(const std::string& name, const std::string& filename);
	
	void unloadAudio(const std::string& name);
	void unloadImage(const std::string& name);
	void unloadModel(const std::string& name);
	
	Audio* getAudio(const std::string& name) const;
	Image* getImage(const std::string& name) const;
	graphics::model::Model* getModel(const std::string& name) const;
	
	ModelHandle loadStaticModel(const graphics::model::Model* model);
	std::shared_future<ModelHandle> loadStaticModelAsync(const graphics::model::Model* model);
	graphics::RenderableHandle createRenderable(
		const graphics::RenderSceneHandle& renderSceneHandle,
		const ModelHandle& modelHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const glm::vec3& scale = glm::vec3(1.0f)
	);
	graphics::RenderableHandle createRenderable(
		const graphics::RenderSceneHandle& renderSceneHandle,
		const graphics::MeshHandle& meshHandle,
		const graphics::TextureHandle& textureHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const glm::vec3& scale = glm::vec3(1.0f)
	);
	
	graphics::VertexShaderHandle createVertexShader(const std::string& name, const std::string& filename);
	std::shared_future<graphics::VertexShaderHandle> createVertexShaderAsync(const std::string& name, const std::string& filename);
	graphics::VertexShaderHandle createVertexShaderFromSource(const std::string& name, const std::string& data);
	std::shared_future<graphics::VertexShaderHandle> createVertexShaderFromSourceAsync(const std::string& name, const std::string& data);
	graphics::FragmentShaderHandle createFragmentShader(const std::string& name, const std::string& filename);
	std::shared_future<graphics::FragmentShaderHandle> createFragmentShaderAsync(const std::string& name, const std::string& filename);
	graphics::FragmentShaderHandle createFragmentShaderFromSource(const std::string& name, const std::string& data);
	std::shared_future<graphics::FragmentShaderHandle> createFragmentShaderFromSourceAsync(const std::string& name, const std::string& data);
	graphics::VertexShaderHandle getVertexShader(const std::string& name) const;
	graphics::FragmentShaderHandle getFragmentShader(const std::string& name) const;
	void destroyShader(const std::string& name);
	void destroyShader(const graphics::VertexShaderHandle& shaderHandle);
	void destroyShader(const graphics::FragmentShaderHandle& shaderHandle);
	
	graphics::ShaderProgramHandle createShaderProgram(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle);
	std::shared_future<graphics::ShaderProgramHandle> createShaderProgramAsync(const std::string& name, const graphics::VertexShaderHandle& vertexShaderHandle, const graphics::FragmentShaderHandle& fragmentShaderHandle);
	graphics::ShaderProgramHandle getShaderProgram(const std::string& name) const;
	void destroyShaderProgram(const std::string& name);
	void destroyShaderProgram(const graphics::ShaderProgramHandle& shaderProgramHandle);
	
	Scene* createScene(const std::string& name);
	void destroyScene(const std::string& name);
	void destroyScene(Scene* scene);
	Scene* getScene(const std::string& name) const;
	std::vector<Scene*> getAllScenes() const;
	
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


	void testSerialize();

	void addWindowEventListener(IWindowEventListener* windowEventListener);
	void addKeyboardEventListener(IKeyboardEventListener* keyboardEventListener);
	void addMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener);
	void addMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener);
	void addMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener);
	void removeWindowEventListener(IWindowEventListener* windowEventListener);
	void removeKeyboardEventListener(IKeyboardEventListener* keyboardEventListener);
	void removeMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener);
	void removeMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener);
	void removeMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener);
	
	void addWindowEventListener(void* windowEventListener);
	void addKeyboardEventListener(void* keyboardEventListener);
	void addMouseMotionEventListener(void* mouseMotionEventListener);
	void addMouseButtonEventListener(void* mouseButtonEventListener);
	void addMouseWheelEventListener(void* mouseWheelEventListener);
	void removeWindowEventListener(void* windowEventListener);
	void removeKeyboardEventListener(void* keyboardEventListener);
	void removeMouseMotionEventListener(void* mouseMotionEventListener);
	void removeMouseButtonEventListener(void* mouseButtonEventListener);
	void removeMouseWheelEventListener(void* mouseWheelEventListener);
	
	void addConnectEventListener(IConnectEventListener* connectEventListener);
	void addDisconnectEventListener(IDisconnectEventListener* disconnectEventListener);
	void addMessageEventListener(IMessageEventListener* messageEventListener);
	void removeConnectEventListener(IConnectEventListener* connectEventListener);
	void removeDisconnectEventListener(IDisconnectEventListener* disconnectEventListener);
	void removeMessageEventListener(IMessageEventListener* messageEventListener);
	
	void addConnectEventListener(void* connectEventListener);
	void addDisconnectEventListener(void* disconnectEventListener);
	void addMessageEventListener(void* messageEventListener);
	void removeConnectEventListener(void* connectEventListener);
	void removeDisconnectEventListener(void* disconnectEventListener);
	void removeMessageEventListener(void* messageEventListener);
	
	std::shared_future<void> postWorkToBackgroundThreadPool(void* object);

	// Implements the graphics::IEventListener interface
	virtual bool processEvent(const graphics::Event& event) override;

	// Implements the networking::IEventListener interface
	virtual bool processEvent(const networking::ConnectEvent& event) override;
	virtual bool processEvent(const networking::DisconnectEvent& event) override;
	virtual bool processEvent(const networking::MessageEvent& event) override;

	template <typename ... Args>
	physics::CollisionShapeHandle createStaticBoxShape(const std::string& name, const Args ... args)
	{
		auto handle = physicsEngine_->createStaticBoxShape(args ...);

		resourceHandleCache_.addCollisionShapeHandle(name, handle);

		return handle;
	}

	template <typename ... Args>
	physics::CollisionShapeHandle createStaticPlaneShape(const std::string& name, const Args ... args)
	{
		auto handle = physicsEngine_->createStaticPlaneShape(args ...);

		resourceHandleCache_.addCollisionShapeHandle(name, handle);

		return handle;
	}

	template <typename ... Args>
	physics::CollisionShapeHandle createStaticTerrainShape(const std::string& name, const Args ... args)
	{
		auto handle = physicsEngine_->createStaticTerrainShape(args ...);

		resourceHandleCache_.addCollisionShapeHandle(name, handle);

		return handle;
	}

	physics::CollisionShapeHandle createStaticTerrainShape(const std::string& name, const Heightfield& heightfield)
	{
		auto handle = physicsEngine_->createStaticTerrainShape(&heightfield);

		resourceHandleCache_.addCollisionShapeHandle(name, handle);

		return handle;
	}

	void destroyStaticShape(const std::string& name)
	{
		auto handle = resourceHandleCache_.getCollisionShapeHandle(name);
		if (handle)
		{
			resourceHandleCache_.removeCollisionShapeHandle(name);

			physicsEngine_->destroyStaticShape(handle);
		}
	}

	void destroyAllStaticShapes()
	{
		resourceHandleCache_.removeAllCollisionShapeHandles();

		physicsEngine_->destroyAllStaticShapes();
	}

	physics::CollisionShapeHandle getStaticShape(const std::string& name) const
	{
		return resourceHandleCache_.getCollisionShapeHandle(name);
	}

		ModelHandle createStaticModel(const std::string& name, const graphics::model::Model* model)
		{
			auto meshHandle = graphicsEngine_->createStaticMesh(model->meshes[0].vertices, model->meshes[0].indices, model->meshes[0].colors, model->meshes[0].normals, model->meshes[0].textureCoordinates);
			auto textureHandle = graphicsEngine_->createTexture2d( static_cast<const graphics::IImage*>(&model->textures[0].image) );

			staticModels_.push_back(std::make_pair(meshHandle, textureHandle));

			auto index = staticModels_.size() - 1;
			auto handle = ModelHandle(index);

			resourceHandleCache_.addModelHandle(name, handle);

			return handle;
		}

		void destroyStaticModel(const std::string& name)
		{
			auto handle = resourceHandleCache_.getModelHandle(name);
			if (handle)
			{
				resourceHandleCache_.removeModelHandle(name);

				auto staticModel = staticModels_[handle.index()];

				//graphicsEngine_->destroy(staticModel->first);
				//graphicsEngine_->destroy(staticModel->second);
			}
		}

		void destroyAllStaticModels()
		{
			resourceHandleCache_.removeAllModelHandles();

			//physicsEngine_->destroyAllStaticModels();
		}

		ModelHandle getStaticModel(const std::string& name) const
		{
			return resourceHandleCache_.getModelHandle(name);
		}

		graphics::TerrainHandle createStaticTerrain(const std::string& name, const HeightMap& heightMap, const SplatMap& splatMap, const DisplacementMap& displacementMap)
				{
					auto handle = graphicsEngine_->createStaticTerrain(&heightMap, &splatMap, &displacementMap);

					resourceHandleCache_.addTerrainHandle(name, handle);

			return handle;
		}

		void destroyStaticTerrain(const std::string& name)
		{
			auto handle = resourceHandleCache_.getTerrainHandle(name);
			if (handle)
			{
				resourceHandleCache_.removeTerrainHandle(name);

				graphicsEngine_->destroy(handle);
			}
		}

		void destroyAllStaticTerrains()
		{
			resourceHandleCache_.removeAllTerrainHandles();

			//graphicsEngine_->destroyAllStaticTerrains();
		}

		graphics::TerrainHandle getStaticTerrain(const std::string& name) const
		{
			return resourceHandleCache_.getTerrainHandle(name);
		}

	template <typename ... Args>
	pathfinding::PolygonMeshHandle createPolygonMesh(const std::string& name, const Args ... args)
	{
		auto handle = pathfindingEngine_->createPolygonMesh(args ...);

		resourceHandleCache_.addPolygonMeshHandle(name, handle);

		return handle;
	}

	pathfinding::PolygonMeshHandle createPolygonMesh(const std::string& name, const PathfindingTerrain& pathfindingTerrain, const pathfinding::PolygonMeshConfig& polygonMeshConfig)
	{
		auto handle = pathfindingEngine_->createPolygonMesh(&pathfindingTerrain, polygonMeshConfig);

		resourceHandleCache_.addPolygonMeshHandle(name, handle);

		return handle;
	}

	void destroyPolygonMesh(const std::string& name)
	{
		auto handle = resourceHandleCache_.getPolygonMeshHandle(name);
		if (handle)
		{
			resourceHandleCache_.removePolygonMeshHandle(name);

			pathfindingEngine_->destroy(handle);
		}
	}

	pathfinding::PolygonMeshHandle getPolygonMesh(const std::string& name) const
	{
		return resourceHandleCache_.getPolygonMeshHandle(name);
	}

	template <typename ... Args>
	pathfinding::NavigationMeshHandle createNavigationMesh(const std::string& name, const Args ... args)
	{
		auto handle = pathfindingEngine_->createNavigationMesh(args ...);

		resourceHandleCache_.addNavigationMeshHandle(name, handle);

		return handle;
	}

	void destroyNavigationMesh(const std::string& name)
	{
		auto handle = resourceHandleCache_.getNavigationMeshHandle(name);
		if (handle)
		{
			resourceHandleCache_.removeNavigationMeshHandle(name);

			pathfindingEngine_->destroy(handle);
		}
	}

	pathfinding::NavigationMeshHandle getNavigationMesh(const std::string& name) const
	{
		return resourceHandleCache_.getNavigationMeshHandle(name);
	}
//
//	template <class R, typename ... Args>
//	pathfinding::PolygonMeshHandle create(Args && ... args)
//	{
//		return pathfinding::PolygonMeshHandle();
//	}

private:
	std::unique_ptr< utilities::Properties > properties_;
	std::unique_ptr< logger::ILogger > logger_;
	std::unique_ptr<fs::IFileSystem> fileSystem_;
	
	std::unique_ptr<ice_engine::IPluginManager> pluginManager_;
	std::vector<std::unique_ptr< IScriptingEngineBinding >> scriptingEngineBindings_;
	std::vector<std::unique_ptr< IModule >> modules_;

	//Graphics
	std::unique_ptr<graphics::IGraphicsEngineFactory> graphicsEngineFactory_;
	std::unique_ptr< graphics::IGraphicsEngine > graphicsEngine_;
	std::vector<std::unique_ptr< graphics::gui::IGui >> guis_;
	
	std::unique_ptr<ITerrainFactory> terrainFactory_;
	
	std::unique_ptr<IDebugRenderer> debugRenderer_;
	
	std::unique_ptr<audio::IAudioEngineFactory> audioEngineFactory_;
	std::unique_ptr< audio::IAudioEngine > audioEngine_;
		
	std::unique_ptr<networking::INetworkingEngineFactory> networkingEngineFactory_;
	std::unique_ptr< networking::INetworkingEngine > networkingEngine_;
	
	std::unique_ptr< physics::IPhysicsEngine > physicsEngine_;
	std::unique_ptr<pathfinding::IPathfindingEngineFactory> pathfindingEngineFactory_;
	std::unique_ptr< pathfinding::IPathfindingEngine > pathfindingEngine_;
	std::unique_ptr<scripting::IScriptingEngine> scriptingEngine_;
	
	std::vector<IWindowEventListener*> windowEventListeners_;
	std::vector<IKeyboardEventListener*> keyboardEventListeners_;
	std::vector<IMouseMotionEventListener*> mouseMotionEventListeners_;
	std::vector<IMouseButtonEventListener*> mouseButtonEventListeners_;
	std::vector<IMouseWheelEventListener*> mouseWheelEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptWindowEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptKeyboardEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseMotionEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseButtonEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseWheelEventListeners_;
	
	std::vector<IConnectEventListener*> connectEventListeners_;
	std::vector<IDisconnectEventListener*> disconnectEventListeners_;
	std::vector<IMessageEventListener*> messageEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptConnectEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptDisconnectEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMessageEventListeners_;
	
	std::vector<std::unique_ptr<Scene>> scenes_;
	
	std::unordered_map<std::string, graphics::VertexShaderHandle> vertexShaderHandles_;
	std::unordered_map<std::string, graphics::FragmentShaderHandle> fragmentShaderHandles_;
	std::unordered_map<std::string, graphics::ShaderProgramHandle> shaderProgramHandles_;
	
	scripting::ModuleHandle bootstrapModuleHandle_;
	scripting::ScriptObjectHandle scriptObjectHandle_;
	
	ResourceCache resourceCache_;
	ResourceHandleCache resourceHandleCache_;
	
	bool running_;	
	EngineStatistics engineStatistics_;

	void tick(const float32 delta);
	void initialize();
	void destroy();
	void exit();

	void handleEvents();

	static float32 rotationX;
	static float32 rotationY;
	static int32 mousePosX;
	static int32 mousePosY;
	
	std::string bootstrapScriptName_;

	// Testing
	void test();
		
	// Initialization stuff
	void initializeLoggingSubSystem();
	void initializeFileSystemSubSystem();
	void initializeSoundSubSystem();
	void initializePhysicsSubSystem();
	void initializePathfindingSubSystem();
	void initializeGraphicsSubSystem();
	void initializeAudioSubSystem();
	void initializeNetworkingSubSystem();
	void initializeInputSubSystem();
	void initializeScriptingSubSystem();
	void initializeThreadingSubSystem();
	void initializeTerrainSubSystem();
	void initializeDataStoreSubSystem();
	void initializeEntitySubSystem();
	void initializeModuleSubSystem();
	
	void loadEssentialGameData();
	void loadUserInterface();
	
	std::mutex temporaryExecutionContextMutex_;
	std::queue<scripting::ExecutionContextHandle> temporaryExecutionContexts_;

	scripting::ExecutionContextHandle acquireTemporaryExecutionContext();
	void releaseTemporaryExecutionContext(const scripting::ExecutionContextHandle& executionContextHandle);

	std::vector<std::pair<graphics::MeshHandle, graphics::TextureHandle>> staticModels_;
	std::vector<graphics::TerrainHandle> staticTerrain_;
	
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
	>>
	postDeserializeCallbacks_;

	// testing
	std::unique_ptr<ThreadPool> backgroundThreadPool_;
	std::unique_ptr<ThreadPool> foregroundThreadPool_;
	std::unique_ptr<OpenGlLoader> openGlLoader_;
	
	//std::unique_ptr<pyliteserializer::SqliteDataStore> dataStore_;
};

}

#endif /* GAMEENGINE_H_ */
