#include <cstring>

#include <Detour/DetourCommon.h>
#include <Detour/DetourMath.h>
#include <Detour/DetourAlloc.h>
#include <Detour/DetourAssert.h>

#include <DebugUtils/RecastDebugDraw.h>
#include <DetourTileCache/DetourTileCacheBuilder.h>
#include <DebugUtils/DetourDebugDraw.h>

#include "pathfinding/recastnavigation/PathfindingEngine.hpp"

namespace ice_engine
{
namespace pathfinding
{
namespace recastnavigation
{

enum RecastNavigationPartitionType
{
	RECAST_NAVIGATION_PARTITION_WATERSHED = 0,
	RECAST_NAVIGATION_PARTITION_MONOTONE,
	RECAST_NAVIGATION_PARTITION_LAYERS,
};

enum RecastNavigationPolyAreas
{
	RECAST_NAVIGATION_POLYAREA_GROUND,
	RECAST_NAVIGATION_POLYAREA_WATER,
	RECAST_NAVIGATION_POLYAREA_ROAD,
	RECAST_NAVIGATION_POLYAREA_DOOR,
	RECAST_NAVIGATION_POLYAREA_GRASS,
	RECAST_NAVIGATION_POLYAREA_JUMP,
};

enum RecastNavigationPolyFlags
{
	RECAST_NAVIGATION_POLYFLAGS_WALK		= 0x01,		// Ability to walk (ground, grass, road)
	RECAST_NAVIGATION_POLYFLAGS_SWIM		= 0x02,		// Ability to swim (water).
	RECAST_NAVIGATION_POLYFLAGS_DOOR		= 0x04,		// Ability to move through doors.
	RECAST_NAVIGATION_POLYFLAGS_JUMP		= 0x08,		// Ability to jump.
	RECAST_NAVIGATION_POLYFLAGS_DISABLED	= 0x10,		// Disabled polygon
	RECAST_NAVIGATION_POLYFLAGS_ALL			= 0xffff	// All abilities.
};

MovementRequestState moveRequestStateToMovementRequestState(const unsigned char moveRequestState)
{
	switch (moveRequestState)
	{
		case DT_CROWDAGENT_TARGET_NONE:
			return MovementRequestState::NONE;

		case DT_CROWDAGENT_TARGET_FAILED:
			return MovementRequestState::FAILED;

		case DT_CROWDAGENT_TARGET_VALID:
			return MovementRequestState::VALID;

		case DT_CROWDAGENT_TARGET_REQUESTING:
			return MovementRequestState::REQUESTING;

		case DT_CROWDAGENT_TARGET_WAITING_FOR_QUEUE:
			return MovementRequestState::WAITING_FOR_QUEUE;

		case DT_CROWDAGENT_TARGET_WAITING_FOR_PATH:
			return MovementRequestState::WAITING_FOR_PATH;

		case DT_CROWDAGENT_TARGET_VELOCITY:
			return MovementRequestState::VELOCITY;

		default:
			assert(false && "We should never get here");
			break;
	}

	return MovementRequestState::NONE;
}

PathfindingEngine::PathfindingEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger)
{
	properties_ = properties;
	fileSystem_ = fileSystem;
	logger_ = logger;
}

PathfindingEngine::PathfindingEngine(const PathfindingEngine& other)
{
}

PathfindingEngine::~PathfindingEngine()
{
}

void PathfindingEngine::tick(const PathfindingSceneHandle& pathfindingSceneHandle, const float32 delta)
{
	//pathfindingScenes_[pathfindingSceneHandle].dynamicsWorld->stepSimulation(delta, 10);
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	
	if (pathfindingScene.debugRendering && debugRenderer_)
	{
		if (pathfindingScene.crowds.size() > 0)
		{
			const auto& crowd = pathfindingScene.crowds[0];
			const auto& navigationMesh = navigationMeshes_[crowd.navigationMeshHandle];
			const auto& polygonMesh = polygonMeshes_[navigationMesh.polygonMeshHandle];

			if (debugRenderer_ && navigationMesh.navMesh)
			{
				//duDebugDrawNavMeshBVTree(debugRenderer_.get(), *navigationMesh.navMesh);
				duDebugDrawNavMeshPolysWithFlags(debugRenderer_.get(), *navigationMesh.navMesh, RECAST_NAVIGATION_POLYFLAGS_DISABLED, duRGBA(0,0,0,128));
				duDebugDrawNavMeshPolysWithFlags(debugRenderer_.get(), *navigationMesh.navMesh, RECAST_NAVIGATION_POLYFLAGS_ALL, duRGBA(64,64,64,128));
			}
			if (debugRenderer_ && navigationMesh.navMeshQuery)
			{
				//duDebugDrawNavMeshNodes(debugRenderer_.get(), *navigationMesh.navMeshQuery);
			}
			if (debugRenderer_ && polygonMesh.polyMesh)
			{
				//duDebugDrawPolyMesh(debugRenderer_.get(), *navigationMesh.polyMesh);
			}

			for (const auto& agent : crowd.agents)
			{
				const dtCrowdAgent* ag = crowd.crowd->getAgent(agent.index);
				if (!ag->active) continue;

				const float radius = ag->params.radius;
				const float* pos = ag->npos;

				/*
				{
					unsigned int col = duRGBA(0,0,0,32);
					if (m_agentDebug.idx == i)
					{
						col = duRGBA(255,0,0,128);
					}

					duDebugDrawCircle(debugRenderer_.get(), pos[0], pos[1], pos[2], radius, col, 2.0f);
				}
				*/

				// Agent cylinders.
				const float height = ag->params.height;

				{
					unsigned int col = duRGBA(220,220,220,128);
					if (ag->targetState == DT_CROWDAGENT_TARGET_REQUESTING || ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_QUEUE)
					{
						col = duLerpCol(col, duRGBA(128,0,255,128), 32);
					}
					else if (ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_PATH)
					{
						col = duLerpCol(col, duRGBA(128,0,255,128), 128);
					}
					else if (ag->targetState == DT_CROWDAGENT_TARGET_FAILED)
					{
						col = duRGBA(255,32,16,128);
					}
					else if (ag->targetState == DT_CROWDAGENT_TARGET_VELOCITY)
					{
						col = duLerpCol(col, duRGBA(64,255,0,128), 128);
					}

					duDebugDrawCylinder(debugRenderer_.get(), pos[0]-radius, pos[1]+radius*0.1f, pos[2]-radius,pos[0]+radius, pos[1]+height, pos[2]+radius, col);
				}

				// Velocity stuff
				const float* vel = ag->vel;
				const float* dvel = ag->dvel;

				{
					unsigned int col = duRGBA(220,220,220,192);
					if (ag->targetState == DT_CROWDAGENT_TARGET_REQUESTING || ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_QUEUE)
					{
						col = duLerpCol(col, duRGBA(128,0,255,192), 32);
					}
					else if (ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_PATH)
					{
						col = duLerpCol(col, duRGBA(128,0,255,192), 128);
					}
					else if (ag->targetState == DT_CROWDAGENT_TARGET_FAILED)
					{
						col = duRGBA(255,32,16,192);
					}
					else if (ag->targetState == DT_CROWDAGENT_TARGET_VELOCITY)
					{
						col = duLerpCol(col, duRGBA(64,255,0,192), 128);
					}

					duDebugDrawCircle(debugRenderer_.get(), pos[0], pos[1]+height, pos[2], radius, col, 2.0f);

					duDebugDrawArrow(debugRenderer_.get(), pos[0],pos[1]+height,pos[2],
						 pos[0]+dvel[0],pos[1]+height+dvel[1],pos[2]+dvel[2],
						 //0.0f, 0.4f, duRGBA(0,192,255,192), (m_agentDebug.idx == i) ? 2.0f : 1.0f);
						 0.0f, 0.4f, duRGBA(0,192,255,192), 1.0f);

					duDebugDrawArrow(debugRenderer_.get(), pos[0],pos[1]+height,pos[2],
									 pos[0]+vel[0],pos[1]+height+vel[1],pos[2]+vel[2],
									 0.0f, 0.4f, duRGBA(0,0,0,160), 2.0f);
				}
			}
		}
	}
	
	for (auto& crowd : pathfindingScene.crowds)
	{
		for (auto& agent : crowd.agents)
		{
			const dtCrowdAgent* ag = crowd.crowd->getAgent(agent.index);
			if (!ag->active) continue;

			if (ag->targetState != agent.moveRequestState)
			{
				agent.moveRequestState = ag->targetState;
				if (agent.movementRequestStateChangeListener) agent.movementRequestStateChangeListener->update(moveRequestStateToMovementRequestState(agent.moveRequestState));
			}
		}

		crowd.crowd->update(delta, nullptr);

		for (const auto& agent : crowd.agents)
		{
			const dtCrowdAgent* ag = crowd.crowd->getAgent(agent.index);
			if (!ag->active) continue;

			if (agent.agentMotionChangeListener) agent.agentMotionChangeListener->update(glm::vec3(ag->npos[0], ag->npos[1], ag->npos[2]));
		}
	}
}

PathfindingSceneHandle PathfindingEngine::createPathfindingScene()
{
	auto pathfindingSceneHandle = pathfindingScenes_.create();
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	
	//pathfindingScene.broadphase = std::make_unique<btDbvtBroadphase>();
	
	return pathfindingSceneHandle;
}

void PathfindingEngine::destroyPathfindingScene(const PathfindingSceneHandle& pathfindingSceneHandle)
{
	pathfindingScenes_.destroy(pathfindingSceneHandle);
}

void PathfindingEngine::setPathfindingDebugRenderer(IPathfindingDebugRenderer* pathfindingDebugRenderer)
{
	debugRenderer_ = std::make_unique<DebugRenderer>(pathfindingDebugRenderer);
}

void PathfindingEngine::setDebugRendering(const PathfindingSceneHandle& pathfindingSceneHandle, const bool enabled)
{
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	pathfindingScene.debugRendering = enabled;
}

PolygonMeshHandle PathfindingEngine::createPolygonMesh(const std::vector<glm::vec3>& vertices, const std::vector<uint32>& indices, const PolygonMeshConfig& polygonMeshConfig)
{
	logger_->info("Creating polygon mesh");
	
	auto polygonMeshHandle = polygonMeshes_.create();
	auto& polygonMesh = polygonMeshes_[polygonMeshHandle];
	
	rcContext ctx = rcContext();
	
	float bmin[3];
	float bmax[3];
	
	rcCalcBounds(&vertices[0].x, vertices.size(), bmin, bmax);
	
	polygonMesh.config.cs = polygonMeshConfig.cellSize;
	polygonMesh.config.ch = polygonMeshConfig.cellHeight;
	polygonMesh.config.walkableSlopeAngle = polygonMeshConfig.walkableSlopeAngle;
	polygonMesh.config.walkableHeight = polygonMeshConfig.walkableHeight;
	polygonMesh.config.walkableClimb = polygonMeshConfig.walkableClimb;
	polygonMesh.config.walkableRadius = polygonMeshConfig.walkableRadius;
	polygonMesh.config.maxEdgeLen = polygonMeshConfig.maxEdgeLength;
	polygonMesh.config.maxSimplificationError = polygonMeshConfig.maxSimplificationError;
	polygonMesh.config.minRegionArea = polygonMeshConfig.minRegionArea;
	polygonMesh.config.mergeRegionArea = polygonMeshConfig.mergeRegionArea;
	polygonMesh.config.maxVertsPerPoly = polygonMeshConfig.maxVertsPerPoly;
	polygonMesh.config.detailSampleDist = polygonMeshConfig.detailSampleDist;
	polygonMesh.config.detailSampleMaxError = polygonMeshConfig.detailSampleMaxError;

	rcVcopy(polygonMesh.config.bmin, bmin);
	rcVcopy(polygonMesh.config.bmax, bmax);
	
	rcCalcGridSize(polygonMesh.config.bmin, polygonMesh.config.bmax, polygonMesh.config.cs, &polygonMesh.config.width, &polygonMesh.config.height);
	
	// Rasterize input
	polygonMesh.heightField = std::unique_ptr<rcHeightfield, decltype(&rcFreeHeightField)>(rcAllocHeightfield(), rcFreeHeightField);
	if (!polygonMesh.heightField)
	{
		throw std::bad_alloc(); //("Unable to create height field - out of memory.");
	}
	
	if (!rcCreateHeightfield(&ctx, *polygonMesh.heightField, polygonMesh.config.width, polygonMesh.config.height, polygonMesh.config.bmin, polygonMesh.config.bmax, polygonMesh.config.cs, polygonMesh.config.ch))
	{
		throw std::runtime_error("Unable to create height field.");
	}
	
	std::vector<unsigned char> triangleAreas = std::vector<unsigned char>(indices.size()/3, 0);
	
	logger_->debug("triangleAreas size: " + std::to_string(triangleAreas.size()));
	
	rcMarkWalkableTriangles(&ctx, polygonMesh.config.walkableSlopeAngle, &vertices[0].x, vertices.size()*3, reinterpret_cast<const int*>(&indices[0]), indices.size()/3, &triangleAreas[0]);
	
	if (!rcRasterizeTriangles(&ctx, &vertices[0].x, vertices.size()*3, reinterpret_cast<const int*>(&indices[0]), &triangleAreas[0], indices.size()/3, *polygonMesh.heightField, polygonMesh.config.walkableClimb))
	{
		throw std::runtime_error("Unable to rasterize triangles.");
	}
	
	// Filter walkables surfaces
	rcFilterLowHangingWalkableObstacles(&ctx, polygonMesh.config.walkableClimb, *polygonMesh.heightField);
	rcFilterLedgeSpans(&ctx, polygonMesh.config.walkableHeight, polygonMesh.config.walkableClimb, *polygonMesh.heightField);
	rcFilterWalkableLowHeightSpans(&ctx, polygonMesh.config.walkableHeight, *polygonMesh.heightField);
	
	if (polygonMesh.heightField->width == 0 || polygonMesh.heightField->height == 0) throw std::runtime_error("Height field has height or width of zero.");
	
	// Partition walkable surface to simple regions
	polygonMesh.compactHeightfield = std::unique_ptr<rcCompactHeightfield, decltype(&rcFreeCompactHeightfield)>(rcAllocCompactHeightfield(), rcFreeCompactHeightfield);
	if (!polygonMesh.compactHeightfield)
	{
		throw std::bad_alloc(); //("Unable to create compact height field - out of memory.");
	}
	
	if (!rcBuildCompactHeightfield(&ctx, polygonMesh.config.walkableHeight, polygonMesh.config.walkableClimb, *polygonMesh.heightField, *polygonMesh.compactHeightfield))
	{
		throw std::runtime_error("Unable to build compact data.");
	}
	
	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(&ctx, polygonMesh.config.walkableRadius, *polygonMesh.compactHeightfield))
	{
		throw std::runtime_error("Unable to erode.");
	}

	// Build regions based on partition type
	RecastNavigationPartitionType partitionType = RECAST_NAVIGATION_PARTITION_WATERSHED;
	if (partitionType == RECAST_NAVIGATION_PARTITION_WATERSHED)
	{
		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!rcBuildDistanceField(&ctx, *polygonMesh.compactHeightfield))
		{
			throw std::runtime_error("Unable to build distance field.");
		}
		
		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(&ctx, *polygonMesh.compactHeightfield, 0, polygonMesh.config.minRegionArea, polygonMesh.config.mergeRegionArea))
		{
			throw std::runtime_error("Unable to build watershed regions.");
		}
	}
	else if (partitionType == RECAST_NAVIGATION_PARTITION_MONOTONE)
	{
		// Partition the walkable surface into simple regions without holes.
		// Monotone partitioning does not need distancefield.
		if (!rcBuildRegionsMonotone(&ctx, *polygonMesh.compactHeightfield, 0, polygonMesh.config.minRegionArea, polygonMesh.config.mergeRegionArea))
		{
			throw std::runtime_error("Unable to build monotone regions.");
		}
	}
	else // RECAST_NAVIGATION_PARTITION_LAYERS
	{
		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildLayerRegions(&ctx, *polygonMesh.compactHeightfield, 0, polygonMesh.config.minRegionArea))
		{
			throw std::runtime_error("Unable to build layer regions.");
		}
	}
	
	if (polygonMesh.compactHeightfield->width == 0 || polygonMesh.compactHeightfield->height == 0) throw std::runtime_error("Compact height field has height or width of zero.");
	if (polygonMesh.compactHeightfield->spanCount == 0) throw std::runtime_error("Compact height field set has no spans.");
	
	// Trace and simplify region contours.
	polygonMesh.contourSet = std::unique_ptr<rcContourSet, decltype(&rcFreeContourSet)>(rcAllocContourSet(), rcFreeContourSet);
	if (!polygonMesh.contourSet)
	{
		throw std::bad_alloc(); //("Unable to create contours - out of memory.");
	}
	if (!rcBuildContours(&ctx, *polygonMesh.compactHeightfield, polygonMesh.config.maxSimplificationError, polygonMesh.config.maxEdgeLen, *polygonMesh.contourSet))
	{
		throw std::runtime_error("Unable to create contours.");
	}
	if (polygonMesh.contourSet->width == 0 || polygonMesh.contourSet->height == 0) throw std::runtime_error("Contour set has height or width of zero.");
	if (polygonMesh.contourSet->nconts == 0) throw std::runtime_error("Contour set has no contours.");
	
	// Build polygons mesh from contours.
	polygonMesh.polyMesh = std::unique_ptr<rcPolyMesh, decltype(&rcFreePolyMesh)>(rcAllocPolyMesh(), rcFreePolyMesh);
	if (!polygonMesh.polyMesh)
	{
		throw std::bad_alloc(); //("Unable to triangulate contours - out of memory.");
	}
	if (!rcBuildPolyMesh(&ctx, *polygonMesh.contourSet, polygonMesh.config.maxVertsPerPoly, *polygonMesh.polyMesh))
	{
		throw std::runtime_error("Unable to triangulate contours.");
	}
	
	// Create detail mesh which allows to access approximate height on each polygon.
	polygonMesh.polyMeshDetail = std::unique_ptr<rcPolyMeshDetail, decltype(&rcFreePolyMeshDetail)>(rcAllocPolyMeshDetail(), rcFreePolyMeshDetail);
	if (!polygonMesh.polyMeshDetail)
	{
		throw std::bad_alloc(); //("Unable to build detail mesh - out of memory.");
	}

	if (!rcBuildPolyMeshDetail(&ctx, *polygonMesh.polyMesh, *polygonMesh.compactHeightfield, polygonMesh.config.detailSampleDist, polygonMesh.config.detailSampleMaxError, *polygonMesh.polyMeshDetail))
	{
		throw std::runtime_error("Unable to build detail mesh.");
	}
	
	return polygonMeshHandle;
}

NavigationMeshHandle PathfindingEngine::createNavigationMesh(const PolygonMeshHandle& polygonMeshHandle, const NavigationMeshConfig& navigationMeshConfig)
{
	logger_->info("Creating navigation mesh");
	
	auto navigationMeshHandle = navigationMeshes_.create();
	auto& navigationMesh = navigationMeshes_[navigationMeshHandle];
	
	navigationMesh.polygonMeshHandle = polygonMeshHandle;

	const auto& polygonMesh = polygonMeshes_[polygonMeshHandle];
	
	// Create Detour data from Recast poly mesh.
	if (polygonMesh.config.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		navigationMesh.navMeshQuery = std::unique_ptr<dtNavMeshQuery, decltype(&dtFreeNavMeshQuery)>(dtAllocNavMeshQuery(), dtFreeNavMeshQuery);
		
		unsigned char* navData = 0;
		int navDataSize = 0;

		// Update poly flags from areas.
		for (int i = 0; i < polygonMesh.polyMesh->npolys; ++i)
		{
			if (polygonMesh.polyMesh->areas[i] == RC_WALKABLE_AREA)
				polygonMesh.polyMesh->areas[i] = RECAST_NAVIGATION_POLYAREA_GROUND;
				
			if (polygonMesh.polyMesh->areas[i] == RECAST_NAVIGATION_POLYAREA_GROUND ||
				polygonMesh.polyMesh->areas[i] == RECAST_NAVIGATION_POLYAREA_GRASS ||
				polygonMesh.polyMesh->areas[i] == RECAST_NAVIGATION_POLYAREA_ROAD)
			{
				polygonMesh.polyMesh->flags[i] = RECAST_NAVIGATION_POLYFLAGS_WALK;
			}
			else if (polygonMesh.polyMesh->areas[i] == RECAST_NAVIGATION_POLYAREA_WATER)
			{
				polygonMesh.polyMesh->flags[i] = RECAST_NAVIGATION_POLYFLAGS_SWIM;
			}
			else if (polygonMesh.polyMesh->areas[i] == RECAST_NAVIGATION_POLYAREA_DOOR)
			{
				polygonMesh.polyMesh->flags[i] = RECAST_NAVIGATION_POLYFLAGS_WALK | RECAST_NAVIGATION_POLYFLAGS_DOOR;
			}
		}

		memset(&navigationMesh.params, 0, sizeof(navigationMesh.params));
		navigationMesh.params.verts = polygonMesh.polyMesh->verts;
		navigationMesh.params.vertCount = polygonMesh.polyMesh->nverts;
		navigationMesh.params.polys = polygonMesh.polyMesh->polys;
		navigationMesh.params.polyAreas = polygonMesh.polyMesh->areas;
		navigationMesh.params.polyFlags = polygonMesh.polyMesh->flags;
		navigationMesh.params.polyCount = polygonMesh.polyMesh->npolys;
		navigationMesh.params.nvp = polygonMesh.polyMesh->nvp;
		navigationMesh.params.detailMeshes = polygonMesh.polyMeshDetail->meshes;
		navigationMesh.params.detailVerts = polygonMesh.polyMeshDetail->verts;
		navigationMesh.params.detailVertsCount = polygonMesh.polyMeshDetail->nverts;
		navigationMesh.params.detailTris = polygonMesh.polyMeshDetail->tris;
		navigationMesh.params.detailTriCount = polygonMesh.polyMeshDetail->ntris;
		navigationMesh.params.offMeshConVerts = nullptr;
		navigationMesh.params.offMeshConRad = nullptr;
		navigationMesh.params.offMeshConDir = nullptr;
		navigationMesh.params.offMeshConAreas = nullptr;
		navigationMesh.params.offMeshConFlags = nullptr;
		navigationMesh.params.offMeshConUserID = nullptr;
		navigationMesh.params.offMeshConCount = 0;
		navigationMesh.params.walkableHeight = navigationMeshConfig.walkableHeight;
		navigationMesh.params.walkableRadius = navigationMeshConfig.walkableRadius;
		navigationMesh.params.walkableClimb = navigationMeshConfig.walkableClimb;
		rcVcopy(navigationMesh.params.bmin, polygonMesh.polyMesh->bmin);
		rcVcopy(navigationMesh.params.bmax, polygonMesh.polyMesh->bmax);
		navigationMesh.params.cs = polygonMesh.config.cs;
		navigationMesh.params.ch = polygonMesh.config.ch;
		navigationMesh.params.buildBvTree = true;
		
		if (navigationMesh.params.nvp > DT_VERTS_PER_POLYGON)
			throw std::runtime_error("Unable to build Detour navmesh 1.");
		if (navigationMesh.params.vertCount >= 0xffff)
			throw std::runtime_error("Unable to build Detour navmesh 2.");
		if (!navigationMesh.params.vertCount || !navigationMesh.params.verts)
		{
			printf("what the hell: %i", navigationMesh.params.vertCount);
			throw std::runtime_error("Unable to build Detour navmesh 3.");
		}
		if (!navigationMesh.params.polyCount || !navigationMesh.params.polys)
			throw std::runtime_error("Unable to build Detour navmesh 4.");
		
		if (!dtCreateNavMeshData(&navigationMesh.params, &navData, &navDataSize))
		{
			throw std::runtime_error("Unable to build Detour navmesh.");
		}
		
		navigationMesh.navMesh = std::unique_ptr<dtNavMesh, decltype(&dtFreeNavMesh)>(dtAllocNavMesh(), dtFreeNavMesh);
		if (!navigationMesh.navMesh)
		{
			dtFree(navData);
			throw std::runtime_error("Unable to create Detour navmesh");
		}
		
		dtStatus status;
		
		status = navigationMesh.navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
		if (dtStatusFailed(status))
		{
			dtFree(navData);
			throw std::runtime_error("Unable to init Detour navmesh");
		}
		
		status = navigationMesh.navMeshQuery->init(navigationMesh.navMesh.get(), 2048);
		if (dtStatusFailed(status))
		{
			throw std::runtime_error("Unable to init Detour navmesh query");
		}
	}
	
	return navigationMeshHandle;
}

CrowdHandle PathfindingEngine::createCrowd(const PathfindingSceneHandle& pathfindingSceneHandle, const NavigationMeshHandle& navigationMeshHandle)
{
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	
	auto crowdHandle = pathfindingScene.crowds.create();
	auto& crowd = pathfindingScene.crowds[crowdHandle];
	
	crowd.crowd = std::unique_ptr<dtCrowd, decltype(&dtFreeCrowd)>(dtAllocCrowd(), dtFreeCrowd);
	crowd.navigationMeshHandle = navigationMeshHandle;
	
	auto& navigationMesh = navigationMeshes_[navigationMeshHandle];
	
	crowd.crowd->init(1000, 0.6, navigationMesh.navMesh.get()); //MAX_AGENTS, m_sample->getAgentRadius(), nav);
	
	// Make polygons with 'disabled' flag invalid.
	crowd.crowd->getEditableFilter(0)->setExcludeFlags(RECAST_NAVIGATION_POLYFLAGS_DISABLED);
	
	// Setup local avoidance params to different qualities.
	dtObstacleAvoidanceParams params;
	// Use mostly default settings, copy from dtCrowd.
	memcpy(&params, crowd.crowd->getObstacleAvoidanceParams(0), sizeof(dtObstacleAvoidanceParams));
	
	// Low (11)
	params.velBias = 0.5f;
	params.adaptiveDivs = 5;
	params.adaptiveRings = 2;
	params.adaptiveDepth = 1;
	crowd.crowd->setObstacleAvoidanceParams(0, &params);
	
	// Medium (22)
	params.velBias = 0.5f;
	params.adaptiveDivs = 5; 
	params.adaptiveRings = 2;
	params.adaptiveDepth = 2;
	crowd.crowd->setObstacleAvoidanceParams(1, &params);
	
	// Good (45)
	params.velBias = 0.5f;
	params.adaptiveDivs = 7;
	params.adaptiveRings = 2;
	params.adaptiveDepth = 3;
	crowd.crowd->setObstacleAvoidanceParams(2, &params);
	
	// High (66)
	params.velBias = 0.5f;
	params.adaptiveDivs = 7;
	params.adaptiveRings = 3;
	params.adaptiveDepth = 3;
	
	crowd.crowd->setObstacleAvoidanceParams(3, &params);
	
	return crowdHandle;
}

void PathfindingEngine::destroy(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle)
{
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	pathfindingScene.crowds.destroy(crowdHandle);
}

AgentHandle PathfindingEngine::createAgent(
	const PathfindingSceneHandle& pathfindingSceneHandle,
	const CrowdHandle& crowdHandle,
	const glm::vec3& position,
	const AgentParams& agentParams,
	std::unique_ptr<IAgentMotionChangeListener> agentMotionChangeListener,
	std::unique_ptr<IMovementRequestStateChangeListener> movementRequestStateChangeListener,
	const UserData& userData
)
{
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	auto& crowd = pathfindingScene.crowds[crowdHandle];
	
	dtCrowdAgentParams ap;
	memset(&ap, 0, sizeof(ap));

	ap.radius = agentParams.radius;
	ap.height = agentParams.height;
	ap.maxAcceleration = agentParams.maxAcceleration;
	ap.maxSpeed = agentParams.maxSpeed;
	ap.collisionQueryRange = agentParams.collisionQueryRange;
	ap.pathOptimizationRange = agentParams.pathOptimizationRange;
	ap.separationWeight = agentParams.separationWeight;

	ap.updateFlags = 0; 
	//if (m_toolParams.m_anticipateTurns)
		ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;
	//if (m_toolParams.m_optimizeVis)
		ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;
	//if (m_toolParams.m_optimizeTopo)
		ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;
	//if (m_toolParams.m_obstacleAvoidance)
		ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;
	//if (m_toolParams.m_separation)
		ap.updateFlags |= DT_CROWD_SEPARATION;
	ap.obstacleAvoidanceType = (unsigned char)3.0f;
	
	int idx = crowd.crowd->addAgent(&position.x, &ap);
	if (idx == -1) throw std::runtime_error("Unable to add agent");
	
	auto agentHandle = crowd.agents.create();
	auto& agent = crowd.agents[agentHandle];
	
	agent.index = idx;
	agent.agentMotionChangeListener = std::move(agentMotionChangeListener);
	agent.movementRequestStateChangeListener = std::move(movementRequestStateChangeListener);
	
	const dtQueryFilter* filter = crowd.crowd->getFilter(0);
	const float* ext = crowd.crowd->getQueryExtents();
	dtPolyRef targetRef;
	float targetPos[3];
	float p[3] = {position.x - 3, position.y, position.z - 1};
	
	const auto& navigationMesh = navigationMeshes_[crowd.navigationMeshHandle];
	
	navigationMesh.navMeshQuery->findNearestPoly(p, ext, filter, &targetRef, targetPos);
	
	if (targetRef)
	{
		crowd.crowd->requestMoveTarget(idx, targetRef, targetPos);
	}
	
	// Init trail
	//AgentTrail* trail = &m_trails[idx];
	//for (int i = 0; i < AGENT_MAX_TRAIL; ++i)
	//	dtVcopy(&trail->trail[i*3], p);
	//trail->htrail = 0;
	
	return agentHandle;
}

void PathfindingEngine::destroy(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle, const AgentHandle& agentHandle)
{
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	auto& crowd = pathfindingScene.crowds[crowdHandle];
	crowd.agents.destroy(agentHandle);
}

void PathfindingEngine::requestMoveTarget(
	const PathfindingSceneHandle& pathfindingSceneHandle,
	const CrowdHandle& crowdHandle,
	const AgentHandle& agentHandle,
	const glm::vec3& position
)
{
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	auto& crowd = pathfindingScene.crowds[crowdHandle];
	const auto& agent = crowd.agents[agentHandle];
	
	const auto& navigationMesh = navigationMeshes_[crowd.navigationMeshHandle];
	
	/*
	int32 entityIndex = -1;
	for (int32 i=0; i < entities_.size(); i++)
	{
		if (entities_[i] == entity)
		{
			entityIndex = i;
			break;
		}
	}
	if (entityIndex == -1)
	{
		LOG_WARN(logger_, "Unable to move agent - agent not found.");
		return;
	}
	*/
	
	const dtQueryFilter* filter = crowd.crowd->getFilter(0);
	const float* ext = crowd.crowd->getQueryExtents();
	dtPolyRef targetRef;
	float targetPos[3];
	float p[3] = {position.x, position.y, position.z};
	
	navigationMesh.navMeshQuery->findNearestPoly(p, ext, filter, &targetRef, targetPos);
	
	if (!targetRef)
	{
		LOG_WARN(logger_, "Unable to move agent - A suitable target position was not found.");
		return;
	}
	
	const auto moveRequestSuccess = crowd.crowd->requestMoveTarget(agent.index, targetRef, targetPos);
	
	if (!moveRequestSuccess) LOG_WARN(logger_, "Unable to move agent - move request failed.");
}

void PathfindingEngine::requestMoveVelocity(
	const PathfindingSceneHandle& pathfindingSceneHandle,
	const CrowdHandle& crowdHandle,
	const AgentHandle& agentHandle,
	const glm::vec3& velocity
)
{
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	auto& crowd = pathfindingScene.crowds[crowdHandle];
	const auto& agent = crowd.agents[agentHandle];

	const auto& navigationMesh = navigationMeshes_[crowd.navigationMeshHandle];
	
	/*
	int32 entityIndex = -1;
	for (int32 i=0; i < entities_.size(); i++)
	{
		if (entities_[i] == entity)
		{
			entityIndex = i;
			break;
		}
	}
	if (entityIndex == -1)
	{
		LOG_WARN(logger_, "Unable to move agent - agent not found.");
		return;
	}
	*/

	const auto moveRequestSuccess = crowd.crowd->requestMoveVelocity(agent.index, &velocity[0]);

	if (!moveRequestSuccess) LOG_WARN(logger_, "Unable to move agent - move velocity request failed.");
}

void PathfindingEngine::setMotionChangeListener(
	const PathfindingSceneHandle& pathfindingSceneHandle,
	const CrowdHandle& crowdHandle,
	const AgentHandle& agentHandle,
	std::unique_ptr<IAgentMotionChangeListener> agentMotionChangeListener
)
{
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	auto& crowd = pathfindingScene.crowds[crowdHandle];
	auto& agent = crowd.agents[agentHandle];
	
	agent.agentMotionChangeListener = std::move(agentMotionChangeListener);
}

void PathfindingEngine::setStateChangeListener(
	const PathfindingSceneHandle& pathfindingSceneHandle,
	const CrowdHandle& crowdHandle,
	const AgentHandle& agentHandle,
	std::unique_ptr<IMovementRequestStateChangeListener> movementRequestStateChangeListener
)
{
	auto& pathfindingScene = pathfindingScenes_[pathfindingSceneHandle];
	auto& crowd = pathfindingScene.crowds[crowdHandle];
	auto& agent = crowd.agents[agentHandle];
	
	agent.movementRequestStateChangeListener = std::move(movementRequestStateChangeListener);
}

void PathfindingEngine::setUserData(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle, const AgentHandle& agentHandle, const UserData& userData)
{
	
}

UserData ud;
UserData& PathfindingEngine::getUserData(const PathfindingSceneHandle& pathfindingSceneHandle, const CrowdHandle& crowdHandle, const AgentHandle& agentHandle) const
{
	return ud;
}

}
}
}
