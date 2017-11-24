#include <Detour/DetourNavMeshBuilder.h>
#include <Detour/DetourCommon.h>
#include <Detour/DetourMath.h>
#include <Detour/DetourAlloc.h>
#include <Detour/DetourAssert.h>

#include "pathfinding/recastnavigation/PathfindingEngine.hpp"

namespace hercules
{
namespace pathfinding
{
namespace recastnavigation
{

std::unique_ptr<rcHeightfield, decltype(&rcFreeHeightField)> rcHeightField_ = std::unique_ptr<rcHeightfield, decltype(&rcFreeHeightField)>(nullptr, rcFreeHeightField);
std::unique_ptr<rcCompactHeightfield, decltype(&rcFreeCompactHeightfield)> rcCompactHeightfield_ = std::unique_ptr<rcCompactHeightfield, decltype(&rcFreeCompactHeightfield)>(nullptr, rcFreeCompactHeightfield);
std::unique_ptr<rcContourSet, decltype(&rcFreeContourSet)> rcContourSet_ = std::unique_ptr<rcContourSet, decltype(&rcFreeContourSet)>(nullptr, rcFreeContourSet);
std::unique_ptr<rcPolyMesh, decltype(&rcFreePolyMesh)> rcPolyMesh_ = std::unique_ptr<rcPolyMesh, decltype(&rcFreePolyMesh)>(nullptr, rcFreePolyMesh);
std::unique_ptr<rcPolyMeshDetail, decltype(&rcFreePolyMeshDetail)> rcPolyMeshDetail_ = std::unique_ptr<rcPolyMeshDetail, decltype(&rcFreePolyMeshDetail)>(nullptr, rcFreePolyMeshDetail);

std::unique_ptr<dtNavMesh, decltype(&dtFreeNavMesh)> dtNavMesh_ = std::unique_ptr<dtNavMesh, decltype(&dtFreeNavMesh)>(nullptr, dtFreeNavMesh);
std::unique_ptr<dtNavMeshQuery, decltype(&dtFreeNavMeshQuery)> dtNavMeshQuery_ = std::unique_ptr<dtNavMeshQuery, decltype(&dtFreeNavMeshQuery)>(nullptr, dtFreeNavMeshQuery);

std::unique_ptr<dtCrowd, decltype(&dtFreeCrowd)> dtCrowd_ = std::unique_ptr<dtCrowd, decltype(&dtFreeCrowd)>(nullptr, dtFreeCrowd);

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
	if (dtCrowd_)
	{
		dtCrowd_->update(delta, nullptr);
		
		for (int i = 0; i < dtCrowd_->getAgentCount(); ++i)
		{
			const dtCrowdAgent* ag = dtCrowd_->getAgent(i);
			if (!ag->active)
			{
				continue;
			}
			
			if (entities_.size() > 0)
			{
				scene_->position(entities_[i], glm::vec3(ag->npos[0], ag->npos[1], ag->npos[2]));
			}
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

void PathfindingEngine::test(std::vector<glm::vec3>& vertices, std::vector<uint32>& indices)
{
	rcContext ctx = rcContext();
	
	float bmin[3];
	float bmax[3];
	
	std::cout << "vertices.size(): " << vertices.size() << std::endl;
	std::cout << "indices.size(): " << indices.size() << std::endl;
	
	rcCalcBounds(&vertices[0].x, vertices.size(), bmin, bmax);
	
	rcConfig config;
	config.cs = 0.30; //m_cellSize;
	config.ch = 0.20; //m_cellHeight;
	config.walkableSlopeAngle = 45; //m_agentMaxSlope;
	config.walkableHeight = (int)ceilf(2.0 / config.ch); //(int)ceilf(m_agentHeight / config.ch);
	config.walkableClimb = (int)floorf(0.9 / config.ch); //(int)floorf(m_agentMaxClimb / config.ch);
	config.walkableRadius = (int)ceilf(0.6 / config.cs); //(int)ceilf(m_agentRadius / config.cs);
	config.maxEdgeLen = (int)(12 / 0.30); //(int)(m_edgeMaxLen / m_cellSize);
	config.maxSimplificationError = 1.3; //m_edgeMaxError;
	config.minRegionArea = (int)rcSqr(8); //(int)rcSqr(m_regionMinSize);		// Note: area = size*size
	config.mergeRegionArea = (int)rcSqr(20); //(int)rcSqr(m_regionMergeSize);	// Note: area = size*size
	config.maxVertsPerPoly = (int)6; //(int)m_vertsPerPoly;
	config.detailSampleDist = 6 < 0.9f ? 0 : 0.30 * 6; //m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
	config.detailSampleMaxError = 0.20 * 1; //m_cellHeight * m_detailSampleMaxError;
	rcVcopy(config.bmin, bmin);
	rcVcopy(config.bmax, bmax);
	
	rcCalcGridSize(config.bmin, config.bmax, config.cs, &config.width, &config.height);
	
	// Rasterize input
	rcHeightField_ = std::unique_ptr<rcHeightfield, decltype(&rcFreeHeightField)>(rcAllocHeightfield(), rcFreeHeightField);
	if (!rcHeightField_)
	{
		throw std::bad_alloc(); //("Unable to create height field - out of memory.");
	}
	
	if (!rcCreateHeightfield(&ctx, *rcHeightField_, config.width, config.height, config.bmin, config.bmax, config.cs, config.ch))
	{
		throw std::runtime_error("Unable to create height field.");
	}
	
	std::vector<unsigned char> triangleAreas = std::vector<unsigned char>(indices.size()/3, 0);
	
	std::cout << "triangleAreas.size(): " << triangleAreas.size() << std::endl;
	
	rcMarkWalkableTriangles(&ctx, config.walkableSlopeAngle, &vertices[0].x, vertices.size()*3, reinterpret_cast<int*>(&indices[0]), indices.size()/3, &triangleAreas[0]);
	
	if (!rcRasterizeTriangles(&ctx, &vertices[0].x, vertices.size()*3, reinterpret_cast<int*>(&indices[0]), &triangleAreas[0], indices.size()/3, *rcHeightField_, config.walkableClimb))
	{
		throw std::runtime_error("Unable to rasterize triangles.");
	}
	
	// Filter walkables surfaces
	rcFilterLowHangingWalkableObstacles(&ctx, config.walkableClimb, *rcHeightField_);
	rcFilterLedgeSpans(&ctx, config.walkableHeight, config.walkableClimb, *rcHeightField_);
	rcFilterWalkableLowHeightSpans(&ctx, config.walkableHeight, *rcHeightField_);
	
	// Partition walkable surface to simple regions
	rcCompactHeightfield_ = std::unique_ptr<rcCompactHeightfield, decltype(&rcFreeCompactHeightfield)>(rcAllocCompactHeightfield(), rcFreeCompactHeightfield);
	if (!rcCompactHeightfield_)
	{
		throw std::bad_alloc(); //("Unable to create compact height field - out of memory.");
	}
	
	if (!rcBuildCompactHeightfield(&ctx, config.walkableHeight, config.walkableClimb, *rcHeightField_, *rcCompactHeightfield_))
	{
		throw std::runtime_error("Unable to build compact data.");
	}
	
	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(&ctx, config.walkableRadius, *rcCompactHeightfield_))
	{
		throw std::runtime_error("Unable to erode.");
	}

	// Build regions based on partition type
	RecastNavigationPartitionType partitionType = RECAST_NAVIGATION_PARTITION_WATERSHED;
	if (partitionType == RECAST_NAVIGATION_PARTITION_WATERSHED)
	{
		// Prepare for region partitioning, by calculating distance field along the walkable surface.
		if (!rcBuildDistanceField(&ctx, *rcCompactHeightfield_))
		{
			throw std::runtime_error("Unable to build distance field.");
		}
		
		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(&ctx, *rcCompactHeightfield_, 0, config.minRegionArea, config.mergeRegionArea))
		{
			throw std::runtime_error("Unable to build watershed regions.");
		}
	}
	else if (partitionType == RECAST_NAVIGATION_PARTITION_MONOTONE)
	{
		// Partition the walkable surface into simple regions without holes.
		// Monotone partitioning does not need distancefield.
		if (!rcBuildRegionsMonotone(&ctx, *rcCompactHeightfield_, 0, config.minRegionArea, config.mergeRegionArea))
		{
			throw std::runtime_error("Unable to build monotone regions.");
		}
	}
	else // RECAST_NAVIGATION_PARTITION_LAYERS
	{
		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildLayerRegions(&ctx, *rcCompactHeightfield_, 0, config.minRegionArea))
		{
			throw std::runtime_error("Unable to build layer regions.");
		}
	}
	
	// Trace and simplify region contours.
	rcContourSet_ = std::unique_ptr<rcContourSet, decltype(&rcFreeContourSet)>(rcAllocContourSet(), rcFreeContourSet);
	if (!rcContourSet_)
	{
		throw std::bad_alloc(); //("Unable to create contours - out of memory.");
	}
	if (!rcBuildContours(&ctx, *rcCompactHeightfield_, config.maxSimplificationError, config.maxEdgeLen, *rcContourSet_))
	{
		throw std::runtime_error("Unable to create contours.");
	}
	
	// Build polygons mesh from contours.
	rcPolyMesh_ = std::unique_ptr<rcPolyMesh, decltype(&rcFreePolyMesh)>(rcAllocPolyMesh(), rcFreePolyMesh);
	if (!rcPolyMesh_)
	{
		throw std::bad_alloc(); //("Unable to triangulate contours - out of memory.");
	}
	if (!rcBuildPolyMesh(&ctx, *rcContourSet_, config.maxVertsPerPoly, *rcPolyMesh_))
	{
		throw std::runtime_error("Unable to triangulate contours.");
	}
	
	// Create detail mesh which allows to access approximate height on each polygon.
	rcPolyMeshDetail_ = std::unique_ptr<rcPolyMeshDetail, decltype(&rcFreePolyMeshDetail)>(rcAllocPolyMeshDetail(), rcFreePolyMeshDetail);
	if (!rcPolyMeshDetail_)
	{
		throw std::bad_alloc(); //("Unable to build detail mesh - out of memory.");
	}

	if (!rcBuildPolyMeshDetail(&ctx, *rcPolyMesh_, *rcCompactHeightfield_, config.detailSampleDist, config.detailSampleMaxError, *rcPolyMeshDetail_))
	{
		throw std::runtime_error("Unable to build detail mesh.");
	}
	
	// Create Detour data from Recast poly mesh.
	if (config.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
	{
		dtNavMeshQuery_ = std::unique_ptr<dtNavMeshQuery, decltype(&dtFreeNavMeshQuery)>(dtAllocNavMeshQuery(), dtFreeNavMeshQuery);
		
		unsigned char* navData = 0;
		int navDataSize = 0;

		// Update poly flags from areas.
		for (int i = 0; i < rcPolyMesh_->npolys; ++i)
		{
			if (rcPolyMesh_->areas[i] == RC_WALKABLE_AREA)
				rcPolyMesh_->areas[i] = RECAST_NAVIGATION_POLYAREA_GROUND;
				
			if (rcPolyMesh_->areas[i] == RECAST_NAVIGATION_POLYAREA_GROUND ||
				rcPolyMesh_->areas[i] == RECAST_NAVIGATION_POLYAREA_GRASS ||
				rcPolyMesh_->areas[i] == RECAST_NAVIGATION_POLYAREA_ROAD)
			{
				rcPolyMesh_->flags[i] = RECAST_NAVIGATION_POLYFLAGS_WALK;
			}
			else if (rcPolyMesh_->areas[i] == RECAST_NAVIGATION_POLYAREA_WATER)
			{
				rcPolyMesh_->flags[i] = RECAST_NAVIGATION_POLYFLAGS_SWIM;
			}
			else if (rcPolyMesh_->areas[i] == RECAST_NAVIGATION_POLYAREA_DOOR)
			{
				rcPolyMesh_->flags[i] = RECAST_NAVIGATION_POLYFLAGS_WALK | RECAST_NAVIGATION_POLYFLAGS_DOOR;
			}
		}

		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts = rcPolyMesh_->verts;
		params.vertCount = rcPolyMesh_->nverts;
		params.polys = rcPolyMesh_->polys;
		params.polyAreas = rcPolyMesh_->areas;
		params.polyFlags = rcPolyMesh_->flags;
		params.polyCount = rcPolyMesh_->npolys;
		params.nvp = rcPolyMesh_->nvp;
		params.detailMeshes = rcPolyMeshDetail_->meshes;
		params.detailVerts = rcPolyMeshDetail_->verts;
		params.detailVertsCount = rcPolyMeshDetail_->nverts;
		params.detailTris = rcPolyMeshDetail_->tris;
		params.detailTriCount = rcPolyMeshDetail_->ntris;
		params.offMeshConVerts = nullptr;
		params.offMeshConRad = nullptr;
		params.offMeshConDir = nullptr;
		params.offMeshConAreas = nullptr;
		params.offMeshConFlags = nullptr;
		params.offMeshConUserID = nullptr;
		params.offMeshConCount = 0;
		params.walkableHeight = 2.0; //m_agentHeight;
		params.walkableRadius = 0.6; //m_agentRadius;
		params.walkableClimb = 0.9; //m_agentMaxClimb;
		rcVcopy(params.bmin, rcPolyMesh_->bmin);
		rcVcopy(params.bmax, rcPolyMesh_->bmax);
		params.cs = config.cs;
		params.ch = config.ch;
		params.buildBvTree = true;
		
		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			throw std::runtime_error("Unable to build Detour navmesh.");
		}
		
		dtNavMesh_ = std::unique_ptr<dtNavMesh, decltype(&dtFreeNavMesh)>(dtAllocNavMesh(), dtFreeNavMesh);
		if (!dtNavMesh_)
		{
			dtFree(navData);
			throw std::runtime_error("Unable to create Detour navmesh");
		}
		
		dtStatus status;
		
		status = dtNavMesh_->init(navData, navDataSize, DT_TILE_FREE_DATA);
		if (dtStatusFailed(status))
		{
			dtFree(navData);
			throw std::runtime_error("Unable to init Detour navmesh");
		}
		
		status = dtNavMeshQuery_->init(dtNavMesh_.get(), 2048);
		if (dtStatusFailed(status))
		{
			throw std::runtime_error("Unable to init Detour navmesh query");
		}
	}
	
	// Crowd stuff
	dtCrowd_ = std::unique_ptr<dtCrowd, decltype(&dtFreeCrowd)>(dtAllocCrowd(), dtFreeCrowd);
	
	dtCrowd_->init(1000, 0.6, dtNavMesh_.get()); //MAX_AGENTS, m_sample->getAgentRadius(), nav);
	
	// Make polygons with 'disabled' flag invalid.
	dtCrowd_->getEditableFilter(0)->setExcludeFlags(RECAST_NAVIGATION_POLYFLAGS_DISABLED);
	
	// Setup local avoidance params to different qualities.
	dtObstacleAvoidanceParams params;
	// Use mostly default settings, copy from dtCrowd.
	memcpy(&params, dtCrowd_->getObstacleAvoidanceParams(0), sizeof(dtObstacleAvoidanceParams));
	
	// Low (11)
	params.velBias = 0.5f;
	params.adaptiveDivs = 5;
	params.adaptiveRings = 2;
	params.adaptiveDepth = 1;
	dtCrowd_->setObstacleAvoidanceParams(0, &params);
	
	// Medium (22)
	params.velBias = 0.5f;
	params.adaptiveDivs = 5; 
	params.adaptiveRings = 2;
	params.adaptiveDepth = 2;
	dtCrowd_->setObstacleAvoidanceParams(1, &params);
	
	// Good (45)
	params.velBias = 0.5f;
	params.adaptiveDivs = 7;
	params.adaptiveRings = 2;
	params.adaptiveDepth = 3;
	dtCrowd_->setObstacleAvoidanceParams(2, &params);
	
	// High (66)
	params.velBias = 0.5f;
	params.adaptiveDivs = 7;
	params.adaptiveRings = 3;
	params.adaptiveDepth = 3;
	
	dtCrowd_->setObstacleAvoidanceParams(3, &params);
}

void PathfindingEngine::test2(entities::Entity entity, IScene* scene, const glm::vec3& position, const glm::quat& orientation)
{
	entities_.push_back(entity);
	scene_ = scene;
	
	dtCrowdAgentParams ap;
	memset(&ap, 0, sizeof(ap));
	ap.radius = 0.6;
	ap.height = 2.0;
	ap.maxAcceleration = 8.0f;
	ap.maxSpeed = 3.5f;
	ap.collisionQueryRange = ap.radius * 12.0f;
	ap.pathOptimizationRange = ap.radius * 30.0f;
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
	ap.separationWeight = 2.0f;
	
	int idx = dtCrowd_->addAgent(&position.x, &ap);
	if (idx != -1)
	{
		const dtQueryFilter* filter = dtCrowd_->getFilter(0);
		const float* ext = dtCrowd_->getQueryExtents();
		dtPolyRef targetRef;
		float targetPos[3];
		float p[3] = {position.x - 3, position.y, position.z - 1};
		
		dtNavMeshQuery_->findNearestPoly(p, ext, filter, &targetRef, targetPos);
		
		if (targetRef)
		{
			dtCrowd_->requestMoveTarget(idx, targetRef, targetPos);
		}
		
		// Init trail
		//AgentTrail* trail = &m_trails[idx];
		//for (int i = 0; i < AGENT_MAX_TRAIL; ++i)
		//	dtVcopy(&trail->trail[i*3], p);
		//trail->htrail = 0;
	}
}

void PathfindingEngine::test3(entities::Entity entity, const glm::vec3& position, const glm::quat& orientation)
{
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
		logger_->warn("Unable to move agent - agent not found.");
		return;
	}
	
	const dtQueryFilter* filter = dtCrowd_->getFilter(0);
	const float* ext = dtCrowd_->getQueryExtents();
	dtPolyRef targetRef;
	float targetPos[3];
	float p[3] = {position.x, position.y, position.z};
	
	dtNavMeshQuery_->findNearestPoly(p, ext, filter, &targetRef, targetPos);
	
	if (!targetRef)
	{
		logger_->warn("Unable to move agent - A suitable target position was not found.");
		return;
	}
	
	auto moveRequestSuccess = dtCrowd_->requestMoveTarget(entityIndex, targetRef, targetPos);
	if (!moveRequestSuccess)
	{
		logger_->warn("Unable to move agent - move request failed.");
	}
}

}
}
}
