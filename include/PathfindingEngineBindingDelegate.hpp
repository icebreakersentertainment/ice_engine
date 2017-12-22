#ifndef PATHFINDINGENGINEBINDINGDELEGATE_H_
#define PATHFINDINGENGINEBINDINGDELEGATE_H_

namespace hercules
{

namespace scripting
{
class IScriptingEngine;
}
namespace pathfinding
{
class IPathfindingEngine;
}
namespace logger
{
class ILogger;
}

class GameEngine;

class PathfindingEngineBindingDelegate
{
public:
	PathfindingEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, pathfinding::IPathfindingEngine* pathfindingEngine);
	~PathfindingEngineBindingDelegate() = default;

	void bind();
	
private:
	logger::ILogger* logger_;
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	pathfinding::IPathfindingEngine* pathfindingEngine_;
};

}

#endif /* PATHFINDINGENGINEBINDINGDELEGATE_H_ */

