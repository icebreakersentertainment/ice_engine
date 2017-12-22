#include <string>

#include "Platform.hpp"
#include "Types.hpp"

#include "pathfinding/IPathfindingEngine.hpp"

#include "PathfindingEngineBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "GameEngine.hpp"

namespace hercules
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
	// IPathfindingEngine
	scriptingEngine_->registerObjectType("IPathfindingEngine", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IPathfindingEngine pathfinding", pathfindingEngine_);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"void test(vectorVec3& in, vectorUInt32& in)",
		asMETHODPR(pathfinding::IPathfindingEngine, test, (std::vector<glm::vec3>&, std::vector<uint32>&), void )
	);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"void test2(Entity, IScene@, const vec3& in, const quat& in)",
		asMETHODPR(pathfinding::IPathfindingEngine, test2, (entities::Entity, IScene*, const glm::vec3&, const glm::quat&), void )
	);
	scriptingEngine_->registerClassMethod(
		"IPathfindingEngine",
		"void test3(Entity, const vec3& in, const quat& in)",
		asMETHODPR(pathfinding::IPathfindingEngine, test3, (entities::Entity, const glm::vec3&, const glm::quat&), void )
	);
}
	
};
