#ifndef SCRIPTINGENGINEBINDINGDELEGATE_H_
#define SCRIPTINGENGINEBINDINGDELEGATE_H_

namespace hercules
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

class GameEngine;

class ScriptingEngineBindingDelegate
{
public:
	ScriptingEngineBindingDelegate(scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine, physics::IPhysicsEngine* physicsEngine);
	virtual ~ScriptingEngineBindingDelegate();

	void bind();
	
private:
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	graphics::IGraphicsEngine* graphicsEngine_;
	physics::IPhysicsEngine* physicsEngine_;
	
};

}

#endif /* SCRIPTINGENGINEBINDINGDELEGATE_H_ */

