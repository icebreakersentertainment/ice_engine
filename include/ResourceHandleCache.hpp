#ifndef RESOURCEHANDLECACHE_H_
#define RESOURCEHANDLECACHE_H_

#include <unordered_map>
#include <utility>
#include <string>
#include <mutex>

#include <boost/thread/shared_mutex.hpp>
#include <boost/thread/shared_lock_guard.hpp>

#include "ModelHandle.hpp"
#include "SkeletonHandle.hpp"
#include "AnimationHandle.hpp"

#include "graphics/MeshHandle.hpp"
#include "graphics/TextureHandle.hpp"
#include "graphics/TerrainHandle.hpp"
#include "graphics/SkyboxHandle.hpp"

#include "audio/SoundHandle.hpp"

#include "physics/CollisionShapeHandle.hpp"

#include "pathfinding/PolygonMeshHandle.hpp"
#include "pathfinding/NavigationMeshHandle.hpp"

#include "exceptions/Exception.hpp"

#include "Types.hpp"

namespace ice_engine
{

class ResourceHandleCache
{
public:
	void addCollisionShapeHandle(const std::string& name, const physics::CollisionShapeHandle& handle)
	{
		std::unique_lock<boost::shared_mutex> lock(collisionShapeHandleMutex_);

		if (collisionShapeHandleMap_.find(name) != collisionShapeHandleMap_.end())
		{
			throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
		}

		collisionShapeHandleMap_[name] = handle;
	}

	void removeCollisionShapeHandle(const std::string& name)
	{
		std::unique_lock<boost::shared_mutex> lock(collisionShapeHandleMutex_);

		auto it = collisionShapeHandleMap_.find(name);
		if (it != collisionShapeHandleMap_.end())
		{
			collisionShapeHandleMap_.erase(it);
		}
	}

	void removeAllCollisionShapeHandles()
	{
		std::unique_lock<boost::shared_mutex> lock(collisionShapeHandleMutex_);

		collisionShapeHandleMap_.clear();
	}

	physics::CollisionShapeHandle getCollisionShapeHandle(const std::string& name) const
	{
		boost::shared_lock_guard<boost::shared_mutex> lock(collisionShapeHandleMutex_);

        const auto it = collisionShapeHandleMap_.find(name);
		if (it != collisionShapeHandleMap_.end()) return it->second;

		return physics::CollisionShapeHandle();
	}

	void addModelHandle(const std::string& name, const ModelHandle& handle)
    {
        std::unique_lock<boost::shared_mutex> lock(modelHandleMutex_);

        if (modelHandleMap_.find(name) != modelHandleMap_.end())
        {
            throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
        }

        modelHandleMap_[name] = handle;
    }

    void removeModelHandle(const std::string& name)
    {
        std::unique_lock<boost::shared_mutex> lock(modelHandleMutex_);

        auto it = modelHandleMap_.find(name);
        if (it != modelHandleMap_.end())
        {
            modelHandleMap_.erase(it);
        }
    }

    void removeAllModelHandles()
    {
        std::unique_lock<boost::shared_mutex> lock(modelHandleMutex_);

        modelHandleMap_.clear();
    }

    ModelHandle getModelHandle(const std::string& name) const
    {
        boost::shared_lock_guard<boost::shared_mutex> lock(modelHandleMutex_);

        const auto it = modelHandleMap_.find(name);
        if (it != modelHandleMap_.end()) return it->second;

        return ModelHandle();
    }

	void addSkeletonHandle(const std::string& name, const SkeletonHandle& handle)
    {
        std::unique_lock<boost::shared_mutex> lock(skeletonHandleMutex_);

        if (skeletonHandleMap_.find(name) != skeletonHandleMap_.end())
        {
            throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
        }

        skeletonHandleMap_[name] = handle;
    }

    void removeSkeletonHandle(const std::string& name)
    {
        std::unique_lock<boost::shared_mutex> lock(skeletonHandleMutex_);

        auto it = skeletonHandleMap_.find(name);
        if (it != skeletonHandleMap_.end())
        {
            skeletonHandleMap_.erase(it);
        }
    }

    void removeAllSkeletonHandles()
    {
        std::unique_lock<boost::shared_mutex> lock(skeletonHandleMutex_);

        skeletonHandleMap_.clear();
    }

    SkeletonHandle getSkeletonHandle(const std::string& name) const
    {
        boost::shared_lock_guard<boost::shared_mutex> lock(skeletonHandleMutex_);

        const auto it = skeletonHandleMap_.find(name);
        if (it != skeletonHandleMap_.end()) return it->second;

        return SkeletonHandle();
    }

	void addAnimationHandle(const std::string& name, const AnimationHandle& handle)
    {
        std::unique_lock<boost::shared_mutex> lock(animationHandleMutex_);

        if (animationHandleMap_.find(name) != animationHandleMap_.end())
        {
            throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
        }

        animationHandleMap_[name] = handle;
    }

    void removeAnimationHandle(const std::string& name)
    {
        std::unique_lock<boost::shared_mutex> lock(animationHandleMutex_);

        auto it = animationHandleMap_.find(name);
        if (it != animationHandleMap_.end())
        {
            animationHandleMap_.erase(it);
        }
    }

    void removeAllAnimationHandles()
    {
        std::unique_lock<boost::shared_mutex> lock(animationHandleMutex_);

        animationHandleMap_.clear();
    }

    AnimationHandle getAnimationHandle(const std::string& name) const
    {
        boost::shared_lock_guard<boost::shared_mutex> lock(animationHandleMutex_);

        const auto it = animationHandleMap_.find(name);
        if (it != animationHandleMap_.end()) return it->second;

        return AnimationHandle();
    }

	void addMeshHandle(const std::string& name, const graphics::MeshHandle& handle)
    {
        std::unique_lock<boost::shared_mutex> lock(meshHandleMutex_);

        if (meshHandleMap_.find(name) != meshHandleMap_.end())
        {
            throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
        }

        meshHandleMap_[name] = handle;
    }

    void removeMeshHandle(const std::string& name)
    {
        std::unique_lock<boost::shared_mutex> lock(meshHandleMutex_);

        auto it = meshHandleMap_.find(name);
        if (it != meshHandleMap_.end())
        {
            meshHandleMap_.erase(it);
        }
    }

    void removeAllMeshHandles()
    {
        std::unique_lock<boost::shared_mutex> lock(meshHandleMutex_);

        meshHandleMap_.clear();
    }

    graphics::MeshHandle getMeshHandle(const std::string& name) const
    {
        boost::shared_lock_guard<boost::shared_mutex> lock(meshHandleMutex_);

        const auto it = meshHandleMap_.find(name);
        if (it != meshHandleMap_.end()) return it->second;

        return graphics::MeshHandle();
    }

	void addTextureHandle(const std::string& name, const graphics::TextureHandle& handle)
    {
        std::unique_lock<boost::shared_mutex> lock(textureHandleMutex_);

        if (textureHandleMap_.find(name) != textureHandleMap_.end())
        {
            throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
        }

        textureHandleMap_[name] = handle;
    }

    void removeTextureHandle(const std::string& name)
    {
        std::unique_lock<boost::shared_mutex> lock(textureHandleMutex_);

        auto it = textureHandleMap_.find(name);
        if (it != textureHandleMap_.end())
        {
            textureHandleMap_.erase(it);
        }
    }

    void removeAllTextureHandles()
    {
        std::unique_lock<boost::shared_mutex> lock(textureHandleMutex_);

        textureHandleMap_.clear();
    }

    graphics::TextureHandle getTextureHandle(const std::string& name) const
    {
        boost::shared_lock_guard<boost::shared_mutex> lock(textureHandleMutex_);

        const auto it = textureHandleMap_.find(name);
        if (it != textureHandleMap_.end()) return it->second;

        return graphics::TextureHandle();
    }

	void addTerrainHandle(const std::string& name, const graphics::TerrainHandle& handle)
    {
        std::unique_lock<boost::shared_mutex> lock(terrainHandleMutex_);

        if (terrainHandleMap_.find(name) != terrainHandleMap_.end())
        {
            throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
        }

        terrainHandleMap_[name] = handle;
    }

    void removeTerrainHandle(const std::string& name)
    {
        std::unique_lock<boost::shared_mutex> lock(terrainHandleMutex_);

        auto it = terrainHandleMap_.find(name);
        if (it != terrainHandleMap_.end())
        {
            terrainHandleMap_.erase(it);
        }
    }

    void removeAllTerrainHandles()
    {
        std::unique_lock<boost::shared_mutex> lock(terrainHandleMutex_);

        terrainHandleMap_.clear();
    }

    graphics::TerrainHandle getTerrainHandle(const std::string& name) const
    {
        boost::shared_lock_guard<boost::shared_mutex> lock(terrainHandleMutex_);

        const auto it = terrainHandleMap_.find(name);
        if (it != terrainHandleMap_.end()) return it->second;

        return graphics::TerrainHandle();
    }

	void addSkyboxHandle(const std::string& name, const graphics::SkyboxHandle& handle)
	{
		std::unique_lock<boost::shared_mutex> lock(skyboxHandleMutex_);

		if (skyboxHandleMap_.find(name) != skyboxHandleMap_.end())
		{
			throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
		}

		skyboxHandleMap_[name] = handle;
	}

	void removeSkyboxHandle(const std::string& name)
	{
		std::unique_lock<boost::shared_mutex> lock(skyboxHandleMutex_);

		auto it = skyboxHandleMap_.find(name);
		if (it != skyboxHandleMap_.end())
		{
			skyboxHandleMap_.erase(it);
		}
	}

	void removeAllSkyboxHandles()
	{
		std::unique_lock<boost::shared_mutex> lock(skyboxHandleMutex_);

		skyboxHandleMap_.clear();
	}

	graphics::SkyboxHandle getSkyboxHandle(const std::string& name) const
	{
		boost::shared_lock_guard<boost::shared_mutex> lock(skyboxHandleMutex_);

        const auto it = skyboxHandleMap_.find(name);
		if (it != skyboxHandleMap_.end()) return it->second;

		return graphics::SkyboxHandle();
	}

	void addSoundHandle(const std::string& name, const audio::SoundHandle& handle)
	{
		std::unique_lock<boost::shared_mutex> lock(soundHandleMutex_);

		if (soundHandleMap_.find(name) != soundHandleMap_.end())
		{
			throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
		}

		soundHandleMap_[name] = handle;
	}

	void removeSoundHandle(const std::string& name)
	{
		std::unique_lock<boost::shared_mutex> lock(soundHandleMutex_);

		auto it = soundHandleMap_.find(name);
		if (it != soundHandleMap_.end())
		{
			soundHandleMap_.erase(it);
		}
	}

	void removeAllSoundHandles()
	{
		std::unique_lock<boost::shared_mutex> lock(soundHandleMutex_);

		soundHandleMap_.clear();
	}

    audio::SoundHandle getSoundHandle(const std::string& name) const
	{
		boost::shared_lock_guard<boost::shared_mutex> lock(soundHandleMutex_);

		const auto it = soundHandleMap_.find(name);
		if (it != soundHandleMap_.end()) return it->second;

		return audio::SoundHandle();
	}

	void addPolygonMeshHandle(const std::string& name, const pathfinding::PolygonMeshHandle& handle)
	{
		std::unique_lock<boost::shared_mutex> lock(polygonMeshHandleMutex_);

		if (polygonMeshHandleMap_.find(name) != polygonMeshHandleMap_.end())
		{
			throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
		}

		polygonMeshHandleMap_[name] = handle;
	}

	void removePolygonMeshHandle(const std::string& name)
	{
		std::unique_lock<boost::shared_mutex> lock(polygonMeshHandleMutex_);

		auto it = polygonMeshHandleMap_.find(name);
		if (it != polygonMeshHandleMap_.end())
		{
			polygonMeshHandleMap_.erase(it);
		}
	}

	void removeAllPolygonMeshHandles()
	{
		std::unique_lock<boost::shared_mutex> lock(polygonMeshHandleMutex_);

		polygonMeshHandleMap_.clear();
	}

	pathfinding::PolygonMeshHandle getPolygonMeshHandle(const std::string& name) const
	{
		boost::shared_lock_guard<boost::shared_mutex> lock(polygonMeshHandleMutex_);

        const auto it = polygonMeshHandleMap_.find(name);
		if (it != polygonMeshHandleMap_.end()) return it->second;

		return pathfinding::PolygonMeshHandle();
	}

	void addNavigationMeshHandle(const std::string& name, const pathfinding::NavigationMeshHandle& handle)
	{
		std::unique_lock<boost::shared_mutex> lock(navigationMeshHandleMutex_);

		if (navigationMeshHandleMap_.find(name) != navigationMeshHandleMap_.end())
		{
			throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
		}

		navigationMeshHandleMap_[name] = handle;
	}

	void removeNavigationMeshHandle(const std::string& name)
	{
		std::unique_lock<boost::shared_mutex> lock(navigationMeshHandleMutex_);

		auto it = navigationMeshHandleMap_.find(name);
		if (it != navigationMeshHandleMap_.end())
		{
			navigationMeshHandleMap_.erase(it);
		}
	}

	void removeAllNavigationMeshHandles()
	{
		std::unique_lock<boost::shared_mutex> lock(navigationMeshHandleMutex_);

		navigationMeshHandleMap_.clear();
	}

	pathfinding::NavigationMeshHandle getNavigationMeshHandle(const std::string& name) const
	{
		boost::shared_lock_guard<boost::shared_mutex> lock(navigationMeshHandleMutex_);

		const auto it = navigationMeshHandleMap_.find(name);
		if (it != navigationMeshHandleMap_.end()) return it->second;

		return pathfinding::NavigationMeshHandle();
	}

	std::unordered_map<std::string, physics::CollisionShapeHandle> collisionShapeHandleMap()
    {
		boost::shared_lock_guard<boost::shared_mutex> lock(collisionShapeHandleMutex_);

        return collisionShapeHandleMap_;
    }

	std::unordered_map<std::string, ModelHandle> modelHandleMap()
    {
		boost::shared_lock_guard<boost::shared_mutex> lock(modelHandleMutex_);

        return modelHandleMap_;
    }

	std::unordered_map<std::string, SkeletonHandle> skeletonHandleMap()
    {
		boost::shared_lock_guard<boost::shared_mutex> lock(skeletonHandleMutex_);

        return skeletonHandleMap_;
    }

	std::unordered_map<std::string, AnimationHandle> animationHandleMap()
    {
		boost::shared_lock_guard<boost::shared_mutex> lock(animationHandleMutex_);

        return animationHandleMap_;
    }

	std::unordered_map<std::string, graphics::MeshHandle> meshHandleMap()
    {
		boost::shared_lock_guard<boost::shared_mutex> lock(meshHandleMutex_);

        return meshHandleMap_;
    }

	std::unordered_map<std::string, graphics::TextureHandle> textureHandleMap()
    {
		boost::shared_lock_guard<boost::shared_mutex> lock(textureHandleMutex_);

        return textureHandleMap_;
    }

	std::unordered_map<std::string, graphics::TerrainHandle> terrainHandleMap()
    {
		boost::shared_lock_guard<boost::shared_mutex> lock(terrainHandleMutex_);

        return terrainHandleMap_;
    }

	std::unordered_map<std::string, graphics::SkyboxHandle> skyboxHandleMap()
    {
		boost::shared_lock_guard<boost::shared_mutex> lock(skyboxHandleMutex_);

			return skyboxHandleMap_;
    }

    std::unordered_map<std::string, audio::SoundHandle> soundHandleMap()
    {
		boost::shared_lock_guard<boost::shared_mutex> lock(soundHandleMutex_);

        return soundHandleMap_;
    }

	std::unordered_map<std::string, pathfinding::PolygonMeshHandle> polygonMeshHandleMap()
    {
        boost::shared_lock_guard<boost::shared_mutex> lock(polygonMeshHandleMutex_);

         return polygonMeshHandleMap_;
    }

	std::unordered_map<std::string, pathfinding::NavigationMeshHandle> navigationMeshHandleMap()
    {
        boost::shared_lock_guard<boost::shared_mutex> lock(navigationMeshHandleMutex_);

        return navigationMeshHandleMap_;
    }

private:
	mutable boost::shared_mutex collisionShapeHandleMutex_;
	mutable boost::shared_mutex modelHandleMutex_;
	mutable boost::shared_mutex skeletonHandleMutex_;
	mutable boost::shared_mutex animationHandleMutex_;
	mutable boost::shared_mutex meshHandleMutex_;
	mutable boost::shared_mutex textureHandleMutex_;
	mutable boost::shared_mutex terrainHandleMutex_;
	mutable boost::shared_mutex skyboxHandleMutex_;
	mutable boost::shared_mutex soundHandleMutex_;
	mutable boost::shared_mutex polygonMeshHandleMutex_;
	mutable boost::shared_mutex navigationMeshHandleMutex_;

	std::unordered_map<std::string, physics::CollisionShapeHandle> collisionShapeHandleMap_;
	std::unordered_map<std::string, ModelHandle> modelHandleMap_;
	std::unordered_map<std::string, SkeletonHandle> skeletonHandleMap_;
	std::unordered_map<std::string, AnimationHandle> animationHandleMap_;
	std::unordered_map<std::string, graphics::MeshHandle> meshHandleMap_;
	std::unordered_map<std::string, graphics::TextureHandle> textureHandleMap_;
	std::unordered_map<std::string, graphics::TerrainHandle> terrainHandleMap_;
	std::unordered_map<std::string, graphics::SkyboxHandle> skyboxHandleMap_;
	std::unordered_map<std::string, audio::SoundHandle> soundHandleMap_;
	std::unordered_map<std::string, pathfinding::PolygonMeshHandle> polygonMeshHandleMap_;
	std::unordered_map<std::string, pathfinding::NavigationMeshHandle> navigationMeshHandleMap_;
};

//template <typename T>
//class ResourceCache
//{
//public:
//    void add(const std::string& name, const T resource)
//    {
//        std::unique_lock<boost::shared_mutex> lock(mutex_);
//
//        if (resourceMap_.find(name) != resourceMap_.end())
//        {
//            throw RuntimeException(detail::format("Resource with name '%s' already exists.", name));
//        }
//
//        resourceMap_[name] = resource;
//    }
//
//    void remove(const std::string& name)
//    {
//        std::unique_lock<boost::shared_mutex> lock(mutex_);
//
//        auto it = resourceMap_.find(name);
//        if (it != resourceMap_.end())
//        {
//            resourceMap_.erase(it);
//        }
//    }
//
//    void removeAll()
//    {
//        std::unique_lock<boost::shared_mutex> lock(mutex_);
//
//        resourceMap_.clear();
//    }
//
//    T get(const std::string& name) const
//    {
//        boost::shared_lock_guard<boost::shared_mutex> lock(mutex_);
//
//        const auto it = resourceMap_.find(name);
//        if (it != resourceMap_.end()) return it->second;
//
//        return T();
//    }
//
//    std::unordered_map<std::string, T> resourceMap() const
//    {
//        boost::shared_lock_guard<boost::shared_mutex> lock(mutex_);
//
//        return resourceMap_;
//    }
//
//private:
//    mutable boost::shared_mutex mutex_;
//
//    std::unordered_map<std::string, T> resourceMap_;
//};

}

#endif /* RESOURCEHANDLECACHE_H_ */
