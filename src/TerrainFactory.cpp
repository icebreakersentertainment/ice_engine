#include "TerrainFactory.hpp"

#include "Terrain.hpp"

namespace ice_engine
{

std::unique_ptr<ITerrain> TerrainFactory::create(
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
)
{
	std::unique_ptr<ITerrain> ptr = std::make_unique< Terrain >( properties, fileSystem, logger, scene, heightMap, splatMap, displacementMap, graphicsEngine, pathfindingEngine, physicsEngine, audioEngine, audioSceneHandle, renderSceneHandle, physicsSceneHandle, pathfindingSceneHandle );
	
	return std::move( ptr );
}

}
