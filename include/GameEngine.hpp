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
#include <typeindex>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/glm.hpp>

#include "graphics/exceptions/GraphicsException.hpp"

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
#include "ITextInputEventListener.hpp"
#include "IMouseMotionEventListener.hpp"
#include "IMouseButtonEventListener.hpp"
#include "IMouseWheelEventListener.hpp"
#include "IConnectEventListener.hpp"
#include "IDisconnectEventListener.hpp"
#include "IMessageEventListener.hpp"

#include "IScriptingEngineDebugHandler.hpp"

#include "graphics/model/Model.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "extras/FpsCamera.hpp"

#include "handles/HandleVector.hpp"

#include "utilities/Properties.hpp"
#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

#include "ResourceCache.hpp"
#include "ResourceHandleCache.hpp"
#include "resources/ResourceManager.hpp"
#include "resources/EngineResourceManager.hpp"
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
#include "Model.hpp"

#include "Animate.hpp"

namespace ice_engine
{

class Scene;

namespace ecs
{
class EntityComponentSystem;
}

class GameEngine : public graphics::IEventListener, public networking::IEventListener, public scripting::IDebugEventListener
{
public:
	GameEngine(
		std::unique_ptr<utilities::Properties> properties,
        std::unique_ptr<fs::IFileSystem> fileSystem,
		std::unique_ptr<ice_engine::IPluginManager> pluginManager,
		std::unique_ptr<ice_engine::logger::ILogger> logger
	);
	GameEngine(
		std::unique_ptr<utilities::Properties> properties,
        std::unique_ptr<fs::IFileSystem> fileSystem,
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

	audio::IAudioEngine* audioEngine() const;
	graphics::IGraphicsEngine* graphicsEngine() const;
	physics::IPhysicsEngine* physicsEngine() const;
	scripting::IScriptingEngine* scriptingEngine() const;
	IDebugRenderer* debugRenderer() const;
	pathfinding::IPathfindingEngine* pathfindingEngine() const;
	IThreadPool* backgroundThreadPool() const;
	IThreadPool* foregroundThreadPool() const;
	IOpenGlLoader* openGlLoader() const;
	IOpenGlLoader* foregroundGraphicsThreadPool() const
	{
		return forgroundGraphicsThreadPool_.get();
	}
	logger::ILogger* logger() const;
	fs::IFileSystem* fileSystem() const;
	ResourceCache& resourceCache()
	{
		return resourceCache_;
	}

	ResourceHandleCache& resourceHandleCache()
	{
		return resourceHandleCache_;
	}

	template <typename T>
    ResourceManager<T>& resourceManager()
    {
        const auto it = resourceManagers_.find(typeid(T));

        if (it != resourceManagers_.end())
        {
            return *dynamic_cast<ResourceManager<T>*>(it->second.get());
        }

        throw RuntimeException(detail::format("No resource manager found for %s", boost::typeindex::type_id<T>().pretty_name()));
    }

	template <typename T>
    EngineResourceManager<T>& engineResourceManager()
    {
        const auto it = engineResourceManagers_.find(typeid(T));

        if (it != engineResourceManagers_.end())
        {
            return *dynamic_cast<EngineResourceManager<T>*>(it->second.get());
        }

        throw RuntimeException(detail::format("No engine resource manager found for %s", boost::typeindex::type_id<T>().pretty_name()));
    }

//    template <>
//    EngineResourceManager<graphics::MeshHandle>& engineResourceManager()
//	{
//        const auto it = engineResourceManagers_.find(typeid(graphics::MeshHandle));
//
//        if (it != engineResourceManagers_.end())
//        {
//            return *dynamic_cast<EngineResourceManager<graphics::MeshHandle>*>(it->second.get());
//        }
//
//        throw RuntimeException(detail::format("No engine resource manager found for %s", boost::typeindex::type_id<graphics::MeshHandle>().pretty_name()));
//	}

	graphics::gui::IGui* createGui(const std::string& name);
	void destroyGui(const graphics::gui::IGui* gui);

	void setCallback(graphics::gui::IButton* button, void* object);
    void setOnChangeCallback(graphics::gui::ITextField* textField, void* object);
    void setOnChangeCallback(graphics::gui::ITextArea* textArea, void* object);
	void setCallback(graphics::gui::IMenuItem* menuItem, void* object);
	void setCallback(graphics::gui::IComboBox* comboBox, void* object);
	void setCallback(graphics::gui::ITreeView* treeView, void* object);

	Audio* loadAudio(const std::string& name, const std::string& filename);
	std::shared_future<Audio*> loadAudioAsync(const std::string& name, const std::string& filename);

	IImage* createImage(const std::string& name, const std::vector<byte>& data, const uint32 width, const uint32 height, const IImage::Format format)
	{
		LOG_DEBUG(logger_, "Creating image: %s", name);

		resourceCache_.addImage( name, std::make_unique<Image>(data, width, height, format) );

		LOG_DEBUG(logger_, "Done creating image: %s", name);

		return resourceCache_.getImage(name);
	}

//	template <typename ... Args>
//	IImage* createImage(const std::string& name, const Args ... args)
//	{
//		LOG_DEBUG(logger_, "Creating image: " + name);
//
//			resourceCache_.addImage( name, std::make_unique<Image>(args ...) );
//
//			LOG_DEBUG(logger_, "Done creating image: " + name);
//
//			return resourceCache_.getImage(name);
//	}

	IImage* loadImage(const std::string& name, const std::string& filename);
	std::shared_future<IImage*> loadImageAsync(const std::string& name, const std::string& filename);
	Model* loadModel(const std::string& name, const std::string& filename);
	std::shared_future<Model*> loadModelAsync(const std::string& name, const std::string& filename);
	Model* importModel(const std::string& name, const std::string& filename);
	std::shared_future<Model*> importModelAsync(const std::string& name, const std::string& filename);

	void unloadAudio(const std::string& name);
	void unloadImage(const std::string& name);
	void unloadModel(const std::string& name);

	Audio* getAudio(const std::string& name) const;
	IImage* getImage(const std::string& name) const;
	Model* getModel(const std::string& name) const;

	ModelHandle loadStaticModel(const Model* model);
	std::shared_future<ModelHandle> loadStaticModelAsync(const Model* model);
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

	Scene* createScene(const std::string& name, const std::vector<std::string>& scriptData = {}, const std::string& initializationFunctionName = "");
	Scene* createScene(const std::string& name, const scripting::ModuleHandle, const std::string& initializationFunctionName = "");
	void destroyScene(const std::string& name);
	void destroyScene(const Scene* scene);
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
				const std::unordered_map<graphics::MeshHandle, graphics::MeshHandle>&,
				const std::unordered_map<graphics::TextureHandle, graphics::TextureHandle>&,
				const std::unordered_map<SkeletonHandle, SkeletonHandle>&,
				const std::unordered_map<AnimationHandle, AnimationHandle>&,
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
	void addTextInputEventListener(ITextInputEventListener* textInputEventListener);
	void addMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener);
	void addMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener);
	void addMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener);
	void removeWindowEventListener(IWindowEventListener* windowEventListener);
	void removeKeyboardEventListener(IKeyboardEventListener* keyboardEventListener);
    void removeTextInputEventListener(ITextInputEventListener* textInputEventListener);
	void removeMouseMotionEventListener(IMouseMotionEventListener* mouseMotionEventListener);
	void removeMouseButtonEventListener(IMouseButtonEventListener* mouseButtonEventListener);
	void removeMouseWheelEventListener(IMouseWheelEventListener* mouseWheelEventListener);

	void addWindowEventListener(void* windowEventListener);
	void addKeyboardEventListener(void* keyboardEventListener);
	void addTextInputEventListener(void* textInputEventListener);
	void addMouseMotionEventListener(void* mouseMotionEventListener);
	void addMouseButtonEventListener(void* mouseButtonEventListener);
	void addMouseWheelEventListener(void* mouseWheelEventListener);
	void removeWindowEventListener(void* windowEventListener);
	void removeKeyboardEventListener(void* keyboardEventListener);
	void removeTextInputEventListener(void* textInputEventListener);
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

	void addScriptingEngineDebugHandler(IScriptingEngineDebugHandler* handler);
	void removeScriptingEngineDebugHandler(const IScriptingEngineDebugHandler* handler);

	void addScriptingEngineDebugHandler(void* object);
	void removeScriptingEngineDebugHandler(const void* object);

	std::shared_future<void> postWorkToBackgroundThreadPool(void* object);
	std::shared_future<void> postWorkToForegroundThreadPool(void* object);
	std::shared_future<void> postWorkToOpenGlWorker(void* object);

	// Implements the graphics::IEventListener interface
	bool processEvent(const graphics::Event& event) override;

	// Implements the networking::IEventListener interface
	bool processEvent(const networking::ConnectEvent& event) override;
	bool processEvent(const networking::DisconnectEvent& event) override;
	bool processEvent(const networking::MessageEvent& event) override;

	physics::CollisionShapeHandle createStaticBoxShape(const std::string& name, const glm::vec3& dimensions)
	{
		auto handle = physicsEngine_->createStaticBoxShape(dimensions);

		resourceHandleCache_.addCollisionShapeHandle(name, handle);

		return handle;
	}

	template <typename ... Args>
	physics::CollisionShapeHandle createStaticBoxShape(const std::string& name, const Args ... args)
	{
		auto handle = physicsEngine_->createStaticBoxShape(args ...);

		resourceHandleCache_.addCollisionShapeHandle(name, handle);

		return handle;
	}

	physics::CollisionShapeHandle createStaticSphereShape(const std::string& name, const float32 radius)
	{
		auto handle = physicsEngine_->createStaticSphereShape(radius);

		resourceHandleCache_.addCollisionShapeHandle(name, handle);

		return handle;
	}

	template <typename ... Args>
	physics::CollisionShapeHandle createStaticSphereShape(const std::string& name, const Args ... args)
	{
		auto handle = physicsEngine_->createStaticSphereShape(args ...);

		resourceHandleCache_.addCollisionShapeHandle(name, handle);

		return handle;
	}

	physics::CollisionShapeHandle createStaticPlaneShape(const std::string& name, const glm::vec3& planeNormal, const float32 planeConstant)
	{
		auto handle = physicsEngine_->createStaticPlaneShape(planeNormal, planeConstant);

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
		auto handle = physicsEngine_->createStaticTerrainShape(heightfield);

		resourceHandleCache_.addCollisionShapeHandle(name, handle);

		return handle;
	}

	void destroyStaticShape(const std::string& name)
	{
		const auto handle = resourceHandleCache_.getCollisionShapeHandle(name);
		if (handle)
		{
			resourceHandleCache_.removeCollisionShapeHandle(name);

            physicsEngine_->destroy(handle);
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

    ModelHandle createStaticModel(const std::string& name, const Model& model)
    {
        auto meshHandle = graphicsEngine_->createStaticMesh(model.meshes()[0]);
        auto textureHandle = graphicsEngine_->createTexture2d(model.textures()[0]);

        staticModels_.push_back(std::make_pair(meshHandle, textureHandle));

        auto index = staticModels_.size() - 1;
        auto handle = ModelHandle(index);

        resourceHandleCache_.addModelHandle(name, handle);

        return handle;
    }

    void destroyStaticModel(const std::string& name)
    {
        const auto handle = resourceHandleCache_.getModelHandle(name);
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

    SkeletonHandle createSkeleton(const std::string& name, const Skeleton& skeleton)
    {
        auto handle = skeletons_.create();
        skeletons_[handle] = Skeleton(skeleton);

        resourceHandleCache_.addSkeletonHandle(name, handle);

        return handle;
    }

    void animateSkeleton(
        std::vector<glm::mat4>& transformations,
        const std::chrono::duration<float32> runningTime,
        const graphics::MeshHandle& meshHandle,
        const AnimationHandle& animationHandle,
        const SkeletonHandle& skeletonHandle
    );

    void animateSkeleton(
        std::vector<glm::mat4>& transformations,
        const std::chrono::duration<float32> runningTime,
        const uint32 startFrame,
        const uint32 endFrame,
        const graphics::MeshHandle& meshHandle,
        const AnimationHandle& animationHandle,
        const SkeletonHandle& skeletonHandle
    );

    void destroySkeleton(const std::string& name)
    {
        const auto handle = resourceHandleCache_.getSkeletonHandle(name);
        if (handle)
        {
            resourceHandleCache_.removeSkeletonHandle(name);

            skeletons_.destroy(handle);

            //graphicsEngine_->destroy(staticSkeleton->first);
            //graphicsEngine_->destroy(staticSkeleton->second);
        }
    }

    void destroyAllSkeletons()
    {
        resourceHandleCache_.removeAllSkeletonHandles();

        //physicsEngine_->destroyAllSkeletons();
    }

    SkeletonHandle getSkeleton(const std::string& name) const
    {
        return resourceHandleCache_.getSkeletonHandle(name);
    }

    void createSkeleton(const graphics::MeshHandle& meshHandle, const graphics::ISkeleton& skeleton)
    {
        graphicsEngine_->createSkeleton(meshHandle, skeleton);
    }

    AnimationHandle createAnimation(const std::string& name, const Animation& animation)
    {
        auto handle = animations_.create();
        animations_[handle] = Animation(animation);

        resourceHandleCache_.addAnimationHandle(name, handle);

        return handle;
    }

    void destroyAnimation(const std::string& name)
    {
        const auto handle = resourceHandleCache_.getAnimationHandle(name);
        if (handle)
        {
            resourceHandleCache_.removeAnimationHandle(name);

            animations_.destroy(handle);
        }
    }

    void destroyAllAnimations()
    {
        resourceHandleCache_.removeAllAnimationHandles();

        //physicsEngine_->destroyAllAnimations();
    }

    AnimationHandle getAnimation(const std::string& name) const
    {
        return resourceHandleCache_.getAnimationHandle(name);
    }

    graphics::MeshHandle createStaticMesh(const std::string& name, const Mesh& mesh)
    {
        auto handle = graphicsEngine_->createStaticMesh(mesh);

        resourceHandleCache_.addMeshHandle(name, handle);

        meshes_[handle] = mesh;

        return handle;
    }

    void destroyStaticMesh(const std::string& name)
    {
        const auto handle = resourceHandleCache_.getMeshHandle(name);
        if (handle)
        {
            resourceHandleCache_.removeMeshHandle(name);

            //auto staticMesh = staticMeshs_[handle.index()];

            //graphicsEngine_->destroy(staticMesh->first);
            //graphicsEngine_->destroy(staticMesh->second);
        }
    }

    void destroyAllStaticMeshes()
    {
        resourceHandleCache_.removeAllMeshHandles();

        //physicsEngine_->destroyAllStaticMeshs();
    }

    graphics::MeshHandle getStaticMesh(const std::string& name) const
    {
        return resourceHandleCache_.getMeshHandle(name);
    }

    uint32 getBoneId(const graphics::MeshHandle meshHandle, const std::string& name) const
    {
        auto it = meshes_.find(meshHandle);

        if (it != meshes_.end())
        {
            const Mesh& mesh = it->second;

            auto boneIndexMapIterator = mesh.boneData().boneIndexMap.find(name);
            if (boneIndexMapIterator != mesh.boneData().boneIndexMap.end())
            {
                return boneIndexMapIterator->second;
            }
        }

        return 0;
    }

    graphics::TextureHandle createTexture(const std::string& name, const Texture& texture)
    {
        auto handle = graphicsEngine_->createTexture2d(texture);

        resourceHandleCache_.addTextureHandle(name, handle);

        return handle;
    }

    void destroyTexture(const std::string& name)
    {
        const auto handle = resourceHandleCache_.getTextureHandle(name);
        if (handle)
        {
            resourceHandleCache_.removeTextureHandle(name);

            //auto staticTexture = staticTextures_[handle.index()];

            //graphicsEngine_->destroy(staticTexture->first);
            //graphicsEngine_->destroy(staticTexture->second);
        }
    }

    void destroyAllTextures()
    {
        resourceHandleCache_.removeAllTextureHandles();

        //physicsEngine_->destroyAllStaticTextures();
    }

    graphics::TextureHandle getTexture(const std::string& name) const
    {
        return resourceHandleCache_.getTextureHandle(name);
    }

    graphics::TerrainHandle createStaticTerrain(const std::string& name, const HeightMap& heightMap, const SplatMap& splatMap, const DisplacementMap& displacementMap)
    {
        auto handle = graphicsEngine_->createStaticTerrain(heightMap, splatMap, displacementMap);

        resourceHandleCache_.addTerrainHandle(name, handle);

        return handle;
    }

    void destroyStaticTerrain(const std::string& name)
    {
        const auto handle = resourceHandleCache_.getTerrainHandle(name);
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

    graphics::SkyboxHandle createStaticSkybox(const std::string& name, const IImage& back, const IImage& down, const IImage& front, const IImage& left, const IImage& right, const IImage& up)
    {
        auto handle = graphicsEngine_->createStaticSkybox(back, down, front, left, right, up);

        resourceHandleCache_.addSkyboxHandle(name, handle);

        return handle;
    }

    void destroyStaticSkybox(const std::string& name)
    {
        const auto handle = resourceHandleCache_.getSkyboxHandle(name);
        if (handle)
        {
            resourceHandleCache_.removeSkyboxHandle(name);

            graphicsEngine_->destroy(handle);
        }
    }

    void destroyAllStaticSkyboxes()
    {
        resourceHandleCache_.removeAllSkyboxHandles();

        //graphicsEngine_->destroyAllStaticSkyboxes();
    }

    graphics::SkyboxHandle getStaticSkybox(const std::string& name) const
    {
        return resourceHandleCache_.getSkyboxHandle(name);
    }

    audio::SoundHandle createSound(const std::string& name, const Audio& audio)
    {
        auto handle = audioEngine_->createSound(audio);

        resourceHandleCache_.addSoundHandle(name, handle);

        return handle;
    }

    void destroySound(const std::string& name)
    {
        const auto handle = resourceHandleCache_.getSoundHandle(name);
        if (handle)
        {
            resourceHandleCache_.removeSoundHandle(name);

            audioEngine_->destroy(handle);
        }
    }

    void destroyAllSound()
    {
        resourceHandleCache_.removeAllSoundHandles();
    }

    audio::SoundHandle getSound(const std::string& name) const
    {
        return resourceHandleCache_.getSoundHandle(name);
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
		const auto handle = resourceHandleCache_.getPolygonMeshHandle(name);
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

	pathfinding::NavigationMeshHandle createNavigationMesh(const std::string& name, const pathfinding::PolygonMeshHandle& polygonMeshHandle, const pathfinding::NavigationMeshConfig& navigationMeshConfig)
	{
		auto handle = pathfindingEngine_->createNavigationMesh(polygonMeshHandle, navigationMeshConfig);

		resourceHandleCache_.addNavigationMeshHandle(name, handle);

		return handle;
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
		const auto handle = resourceHandleCache_.getNavigationMeshHandle(name);
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
    std::mutex guisCreatedMutex_;
    std::vector<std::unique_ptr<graphics::gui::IGui>> guisCreated_;
    std::mutex guisDeletedMutex_;
    std::vector<const graphics::gui::IGui*> guisDeleted_;

	std::unique_ptr<ITerrainFactory> terrainFactory_;

	std::unique_ptr<IDebugRenderer> debugRenderer_;

	std::unique_ptr<audio::IAudioEngineFactory> audioEngineFactory_;
	std::unique_ptr< audio::IAudioEngine > audioEngine_;

	std::unique_ptr<networking::INetworkingEngineFactory> networkingEngineFactory_;
	std::unique_ptr< networking::INetworkingEngine > networkingEngine_;

	std::unique_ptr<physics::IPhysicsEngineFactory> physicsEngineFactory_;
	std::unique_ptr< physics::IPhysicsEngine > physicsEngine_;
	std::unique_ptr<pathfinding::IPathfindingEngineFactory> pathfindingEngineFactory_;
	std::unique_ptr< pathfinding::IPathfindingEngine > pathfindingEngine_;
	std::unique_ptr<scripting::IScriptingEngine> scriptingEngine_;

	std::vector<IWindowEventListener*> windowEventListeners_;
	std::vector<IKeyboardEventListener*> keyboardEventListeners_;
	std::vector<ITextInputEventListener*> textInputEventListeners_;
	std::vector<IMouseMotionEventListener*> mouseMotionEventListeners_;
	std::vector<IMouseButtonEventListener*> mouseButtonEventListeners_;
	std::vector<IMouseWheelEventListener*> mouseWheelEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptWindowEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptKeyboardEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptTextInputEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseMotionEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseButtonEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMouseWheelEventListeners_;

	std::vector<IConnectEventListener*> connectEventListeners_;
	std::vector<IDisconnectEventListener*> disconnectEventListeners_;
	std::vector<IMessageEventListener*> messageEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptConnectEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptDisconnectEventListeners_;
	std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptMessageEventListeners_;

    std::vector<IScriptingEngineDebugHandler*> scriptingEngineDebugHandlers_;
    std::vector<std::pair<scripting::ScriptObjectHandle, scripting::ScriptObjectFunctionHandle>> scriptScriptingEngineDebugHandlers_;

	std::unordered_map<graphics::MeshHandle, Mesh> meshes_;
	handles::HandleVector<Skeleton, SkeletonHandle> skeletons_;
	handles::HandleVector<Animation, AnimationHandle> animations_;

	std::vector<std::unique_ptr<Scene>> scenes_;

	std::unordered_map<std::string, graphics::VertexShaderHandle> vertexShaderHandles_;
	std::unordered_map<std::string, graphics::FragmentShaderHandle> fragmentShaderHandles_;
	std::unordered_map<std::string, graphics::ShaderProgramHandle> shaderProgramHandles_;

	scripting::ModuleHandle bootstrapModuleHandle_;
	scripting::ScriptObjectHandle scriptObjectHandle_;

	ResourceCache resourceCache_;
	ResourceHandleCache resourceHandleCache_;
    std::map<std::type_index, std::unique_ptr<BaseResourceManager>> resourceManagers_;
    std::map<std::type_index, std::unique_ptr<BaseEngineResourceManager>> engineResourceManagers_;

	bool running_;
	EngineStatistics engineStatistics_;

	void tick(const float32 delta);
    void render();
	void initialize();
	void destroy();
	void exit();

	void handleEvents();

    void internalDestroyGui(const graphics::gui::IGui* gui);

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
    void initializeResourceManagers();
	void initializeEngineResourceManagers();

	void internalInitializeScene(std::unique_ptr<Scene>& scene);

	// Script debugging stuff
    void processEvent(const scripting::DebugEvent& event) override;

	std::mutex temporaryExecutionContextMutex_;
	std::queue<scripting::ExecutionContextHandle> temporaryExecutionContexts_;

	scripting::ExecutionContextHandle debuggerExecutionContext_;

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
			const std::unordered_map<graphics::MeshHandle, graphics::MeshHandle>&,
			const std::unordered_map<graphics::TextureHandle, graphics::TextureHandle>&,
			const std::unordered_map<SkeletonHandle, SkeletonHandle>&,
			const std::unordered_map<AnimationHandle, AnimationHandle>&,
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
	std::unique_ptr<OpenGlLoader> forgroundGraphicsThreadPool_;

	//std::unique_ptr<pyliteserializer::SqliteDataStore> dataStore_;
};

}

#endif /* GAMEENGINE_H_ */
