#ifndef TERRAIN_H_
#define TERRAIN_H_

#include "ITerrain.hpp"

#include "graphics/IGraphicsEngine.hpp"
#include "pathfinding/IPathfindingEngine.hpp"
#include "physics/IPhysicsEngine.hpp"
#include "audio/IAudioEngine.hpp"

#include "ecs/Entity.hpp"

#include "HeightMap.hpp"
#include "SplatMap.hpp"
#include "DisplacementMap.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{

class Scene;

class Terrain : public ITerrain
{
public:
	Terrain(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger,
		Scene* scene,
		HeightMap heightMap,
		SplatMap splatMap,
		DisplacementMap displacementMap,
		physics::CollisionShapeHandle collisionShapeHandle,
		pathfinding::PolygonMeshHandle polygonMeshHandle,
			pathfinding::NavigationMeshHandle navigationMeshHandle,
		graphics::IGraphicsEngine* graphicsEngine,
		pathfinding::IPathfindingEngine* pathfindingEngine,
		physics::IPhysicsEngine* physicsEngine,
		audio::IAudioEngine* audioEngine,
		audio::AudioSceneHandle audioSceneHandle,
		graphics::RenderSceneHandle renderSceneHandle,
		physics::PhysicsSceneHandle physicsSceneHandle,
		pathfinding::PathfindingSceneHandle pathfindingSceneHandle
	);
	virtual ~Terrain();
	
	virtual void tick(const float32 delta) override;
	virtual const std::vector<pathfinding::CrowdHandle>& crowds() const override;

private:
	audio::IAudioEngine* audioEngine_;
	graphics::IGraphicsEngine* graphicsEngine_;
	physics::IPhysicsEngine* physicsEngine_;
	pathfinding::IPathfindingEngine* pathfindingEngine_;
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	Scene* scene_;
	ecs::Entity terrainEntity_;
	
	graphics::TerrainHandle terrainHandle_;
	
	pathfinding::PolygonMeshHandle polygonMeshHandle_;
	pathfinding::NavigationMeshHandle navigationMeshHandle_;

	std::vector<pathfinding::CrowdHandle> crowdHandles_;

	audio::AudioSceneHandle audioSceneHandle_;
	graphics::RenderSceneHandle renderSceneHandle_;
	physics::PhysicsSceneHandle physicsSceneHandle_;
	pathfinding::PathfindingSceneHandle pathfindingSceneHandle_;
};

}

#endif /* TERRAIN_H_ */
