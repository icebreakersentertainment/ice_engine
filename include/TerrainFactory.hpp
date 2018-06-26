#ifndef TERRAINFACTORY_H_
#define TERRAINFACTORY_H_

#include "ITerrainFactory.hpp"

namespace ice_engine
{

class TerrainFactory : public ITerrainFactory
{
public:
	TerrainFactory() = default;
	virtual ~TerrainFactory() = default;

	virtual std::unique_ptr<ITerrain> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger,
		Scene* scene,
		HeightMap heightMap,
		SplatMap splatMap,
		DisplacementMap displacementMap,
		graphics::IGraphicsEngine* graphicsEngine,
		pathfinding::IPathfindingEngine* pathfindingEngine,
		physics::IPhysicsEngine* physicsEngine,
		audio::IAudioEngine* audioEngine,
		audio::AudioSceneHandle audioSceneHandle,
		graphics::RenderSceneHandle renderSceneHandle,
		physics::PhysicsSceneHandle physicsSceneHandle,
		pathfinding::PathfindingSceneHandle pathfindingSceneHandle
	) override;

private:
	TerrainFactory(const TerrainFactory& other) = delete;
};

}

#endif /* TERRAINFACTORY_H_ */
