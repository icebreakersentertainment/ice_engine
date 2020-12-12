#ifndef BINDINGDELEGATE_H_
#define BINDINGDELEGATE_H_

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

class  BindingDelegate
{
public:
	BindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine, audio::IAudioEngine* audioEngine, networking::INetworkingEngine* networkingEngine, physics::IPhysicsEngine* physicsEngine, pathfinding::IPathfindingEngine* pathfindingEngine);

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

#endif /* BINDINGDELEGATE_H_ */

