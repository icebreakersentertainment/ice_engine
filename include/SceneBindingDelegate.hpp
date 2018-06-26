#ifndef SCENEBINDINGDELEGATE_H_
#define SCENEBINDINGDELEGATE_H_

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
namespace audio
{
class IAudioEngine;
}
namespace networking
{
class INetworkingEngine;
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

class SceneBindingDelegate
{
public:
	SceneBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine, audio::IAudioEngine* audioEngine, networking::INetworkingEngine* networkingEngine, physics::IPhysicsEngine* physicsEngine, pathfinding::IPathfindingEngine* pathfindingEngine);
	virtual ~SceneBindingDelegate();

	void bind();
	
private:
	logger::ILogger* logger_;
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	graphics::IGraphicsEngine* graphicsEngine_;
	audio::IAudioEngine* audioEngine_;
	networking::INetworkingEngine* networkingEngine_;
	physics::IPhysicsEngine* physicsEngine_;
	pathfinding::IPathfindingEngine* pathfindingEngine_;
	
};

}

#endif /* SCENEBINDINGDELEGATE_H_ */

