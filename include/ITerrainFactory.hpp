#ifndef ITERRAINFACTORY_H_
#define ITERRAINFACTORY_H_

#include <memory>

#include "ITerrain.hpp"

#include "graphics/IGraphicsEngine.hpp"
#include "pathfinding/IPathfindingEngine.hpp"
#include "physics/IPhysicsEngine.hpp"
#include "audio/IAudioEngine.hpp"

#include "HeightMap.hpp"
#include "SplatMap.hpp"
#include "DisplacementMap.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{

class GameEngine;
class Scene;

class ITerrainFactory
{
public:
	virtual ~ITerrainFactory()
	{
	}
	;
	
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
	) = 0;
};

}

#endif /* ITERRAINFACTORY_H_ */
