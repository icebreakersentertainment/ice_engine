#include <string>

#include "Platform.hpp"
#include "Types.hpp"

#include "pathfinding/IPathfindingEngine.hpp"

#include "PathfindingEngineBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "GameEngine.hpp"

namespace ice_engine
{

PathfindingEngineBindingDelegate::PathfindingEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, pathfinding::IPathfindingEngine* pathfindingEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine),
	pathfindingEngine_(pathfindingEngine)
{
}

void PathfindingEngineBindingDelegate::bind()
{
	registerHandleBindings<pathfinding::PathfindingSceneHandle>(scriptingEngine_, "PathfindingSceneHandle");
	registerHandleBindings<pathfinding::PolygonMeshHandle>(scriptingEngine_, "PolygonMeshHandle");
	registerHandleBindings<pathfinding::NavigationMeshHandle>(scriptingEngine_, "NavigationMeshHandle");
	registerHandleBindings<pathfinding::AgentHandle>(scriptingEngine_, "AgentHandle");
	registerHandleBindings<pathfinding::CrowdHandle>(scriptingEngine_, "CrowdHandle");
	
	scriptingEngine_->registerObjectType("IPathfindingTerrain", 0, asOBJ_REF | asOBJ_NOCOUNT);

	scriptingEngine_->registerObjectType("PolygonMeshConfig", sizeof(pathfinding::PolygonMeshConfig), asOBJ_VALUE | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<pathfinding::PolygonMeshConfig>());
	scriptingEngine_->registerObjectBehaviour("PolygonMeshConfig", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<pathfinding::PolygonMeshConfig>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PolygonMeshConfig", asBEHAVE_CONSTRUCT, "void f(const PolygonMeshConfig& in)", asFUNCTION(CopyConstructor<pathfinding::PolygonMeshConfig>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("PolygonMeshConfig", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<pathfinding::PolygonMeshConfig>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int width", asOFFSET(pathfinding::PolygonMeshConfig, width));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int height", asOFFSET(pathfinding::PolygonMeshConfig, height));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int tileSize", asOFFSET(pathfinding::PolygonMeshConfig, tileSize));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int borderSize", asOFFSET(pathfinding::PolygonMeshConfig, borderSize));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "float cellSize", asOFFSET(pathfinding::PolygonMeshConfig, cellSize));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "float cellHeight", asOFFSET(pathfinding::PolygonMeshConfig, cellHeight));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "float walkableSlopeAngle", asOFFSET(pathfinding::PolygonMeshConfig, walkableSlopeAngle));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int walkableHeight", asOFFSET(pathfinding::PolygonMeshConfig, walkableHeight));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int walkableClimb", asOFFSET(pathfinding::PolygonMeshConfig, walkableClimb));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int walkableRadius", asOFFSET(pathfinding::PolygonMeshConfig, walkableRadius));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int maxEdgeLength", asOFFSET(pathfinding::PolygonMeshConfig, maxEdgeLength));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "float maxSimplificationError", asOFFSET(pathfinding::PolygonMeshConfig, maxSimplificationError));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int minRegionArea", asOFFSET(pathfinding::PolygonMeshConfig, minRegionArea));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int mergeRegionArea", asOFFSET(pathfinding::PolygonMeshConfig, mergeRegionArea));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "int maxVertsPerPoly", asOFFSET(pathfinding::PolygonMeshConfig, maxVertsPerPoly));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "float detailSampleDist", asOFFSET(pathfinding::PolygonMeshConfig, detailSampleDist));
	scriptingEngine_->registerObjectProperty("PolygonMeshConfig", "float detailSampleMaxError", asOFFSET(pathfinding::PolygonMeshConfig, detailSampleMaxError));

	scriptingEngine_->registerObjectType("NavigationMeshConfig", sizeof(pathfinding::NavigationMeshConfig), asOBJ_VALUE | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<pathfinding::NavigationMeshConfig>());
	scriptingEngine_->registerObjectBehaviour("NavigationMeshConfig", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<pathfinding::NavigationMeshConfig>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("NavigationMeshConfig", asBEHAVE_CONSTRUCT, "void f(const NavigationMeshConfig& in)", asFUNCTION(CopyConstructor<pathfinding::NavigationMeshConfig>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("NavigationMeshConfig", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<pathfinding::NavigationMeshConfig>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectProperty("NavigationMeshConfig", "float walkableHeight", asOFFSET(pathfinding::NavigationMeshConfig, walkableHeight));
	scriptingEngine_->registerObjectProperty("NavigationMeshConfig", "float walkableClimb", asOFFSET(pathfinding::NavigationMeshConfig, walkableClimb));
	scriptingEngine_->registerObjectProperty("NavigationMeshConfig", "float walkableRadius", asOFFSET(pathfinding::NavigationMeshConfig, walkableRadius));

	scriptingEngine_->registerObjectType("AgentParams", sizeof(pathfinding::AgentParams), asOBJ_VALUE | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<pathfinding::AgentParams>());
	scriptingEngine_->registerObjectBehaviour("AgentParams", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<pathfinding::AgentParams>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("AgentParams", asBEHAVE_CONSTRUCT, "void f(const AgentParams& in)", asFUNCTION(CopyConstructor<pathfinding::AgentParams>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("AgentParams", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<pathfinding::AgentParams>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectProperty("AgentParams", "float radius", asOFFSET(pathfinding::AgentParams, radius));
	scriptingEngine_->registerObjectProperty("AgentParams", "float height", asOFFSET(pathfinding::AgentParams, height));
	scriptingEngine_->registerObjectProperty("AgentParams", "float maxAcceleration", asOFFSET(pathfinding::AgentParams, maxAcceleration));
	scriptingEngine_->registerObjectProperty("AgentParams", "float maxSpeed", asOFFSET(pathfinding::AgentParams, maxSpeed));
	scriptingEngine_->registerObjectProperty("AgentParams", "float collisionQueryRange", asOFFSET(pathfinding::AgentParams, collisionQueryRange));
	scriptingEngine_->registerObjectProperty("AgentParams", "float pathOptimizationRange", asOFFSET(pathfinding::AgentParams, pathOptimizationRange));
	scriptingEngine_->registerObjectProperty("AgentParams", "float separationWeight", asOFFSET(pathfinding::AgentParams, separationWeight));

	scriptingEngine_->registerEnum("MovementRequestState");
	scriptingEngine_->registerEnumValue("MovementRequestState", "NONE", pathfinding::NONE);
	scriptingEngine_->registerEnumValue("MovementRequestState", "FAILED", pathfinding::FAILED);
	scriptingEngine_->registerEnumValue("MovementRequestState", "VALID", pathfinding::VALID);
	scriptingEngine_->registerEnumValue("MovementRequestState", "REQUESTING", pathfinding::REQUESTING);
	scriptingEngine_->registerEnumValue("MovementRequestState", "WAITING_FOR_QUEUE", pathfinding::WAITING_FOR_QUEUE);
	scriptingEngine_->registerEnumValue("MovementRequestState", "WAITING_FOR_PATH", pathfinding::WAITING_FOR_PATH);
	scriptingEngine_->registerEnumValue("MovementRequestState", "VELOCITY", pathfinding::VELOCITY);

	scriptingEngine_->registerEnum("AgentState");
	scriptingEngine_->registerEnumValue("AgentState", "INVALID", pathfinding::AgentState::INVALID);
	scriptingEngine_->registerEnumValue("AgentState", "WALKING", pathfinding::AgentState::WALKING);
	scriptingEngine_->registerEnumValue("AgentState", "OFFMESH", pathfinding::AgentState::OFFMESH);

	// IPathfindingEngine
	scriptingEngine_->registerObjectType("IPathfindingEngine", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IPathfindingEngine pathfinding", pathfindingEngine_);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"PolygonMeshHandle createPolygonMesh(const IPathfindingTerrain@, const PolygonMeshConfig& in)",
		asMETHODPR(pathfinding::IPathfindingEngine, createPolygonMesh, (const pathfinding::ITerrain*, const pathfinding::PolygonMeshConfig&), pathfinding::PolygonMeshHandle )
	);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"NavigationMeshHandle createNavigationMesh(const PolygonMeshHandle& in, const NavigationMeshConfig& in)",
		asMETHODPR(pathfinding::IPathfindingEngine, createNavigationMesh, (const pathfinding::PolygonMeshHandle&, const pathfinding::NavigationMeshConfig&), pathfinding::NavigationMeshHandle )
	);
	/*
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"CrowdHandle createCrowd(const PathfindingSceneHandle& in, const NavigationMeshHandle& in)",
		asMETHODPR(pathfinding::IPathfindingEngine, createCrowd, (const pathfinding::PathfindingSceneHandle&, const pathfinding::NavigationMeshHandle&), pathfinding::CrowdHandle )
	);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"AgentHandle createAgent(const PathfindingSceneHandle& in, const CrowdHandle& in, const vec3& in, )",
		asMETHODPR(pathfinding::IPathfindingEngine, createAgent, (const pathfinding::PathfindingSceneHandle&, const pathfinding::CrowdHandle&, const glm::vec3&), pathfinding::AgentHandle )
	);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"void requestMoveTarget(const PathfindingSceneHandle& in, const CrowdHandle& in, const AgentHandle&, const vec3& in, )",
		asMETHODPR(pathfinding::IPathfindingEngine, requestMoveTarget, (const pathfinding::PathfindingSceneHandle&, const pathfinding::CrowdHandle&, const pathfinding::AgentHandle&, const glm::vec3&), void )
	);
	*/
}
	
};
