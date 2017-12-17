#ifndef SCRIPTINGENGINEBINDINGDELEGATE_H_
#define SCRIPTINGENGINEBINDINGDELEGATE_H_

namespace ice_engine
{

namespace scripting
{
class IScriptingEngine;
}
namespace graphics
{
class IGraphicsEngine;
}
namespace physics
{
class IPhysicsEngine;
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

class ScriptingEngineBindingDelegate
{
public:
	ScriptingEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine, physics::IPhysicsEngine* physicsEngine, pathfinding::IPathfindingEngine* pathfindingEngine);
	virtual ~ScriptingEngineBindingDelegate();

	void bind();
	
private:
	logger::ILogger* logger_;
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	graphics::IGraphicsEngine* graphicsEngine_;
	physics::IPhysicsEngine* physicsEngine_;
	pathfinding::IPathfindingEngine* pathfindingEngine_;
	
};

}

#endif /* SCRIPTINGENGINEBINDINGDELEGATE_H_ */

