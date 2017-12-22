#ifndef PHYSICSENGINEBINDINGDELEGATE_H_
#define PHYSICSENGINEBINDINGDELEGATE_H_

namespace ice_engine
{

namespace scripting
{
class IScriptingEngine;
}
namespace physics
{
class IPhysicsEngine;
}
namespace logger
{
class ILogger;
}

class GameEngine;

class PhysicsEngineBindingDelegate
{
public:
	PhysicsEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, physics::IPhysicsEngine* physicsEngine);
	~PhysicsEngineBindingDelegate() = default;

	void bind();
	
private:
	logger::ILogger* logger_;
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	physics::IPhysicsEngine* physicsEngine_;
};

}

#endif /* PHYSICSENGINEBINDINGDELEGATE_H_ */

