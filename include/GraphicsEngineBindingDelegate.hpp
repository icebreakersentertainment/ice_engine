#ifndef GRAPHICSENGINEBINDINGDELEGATE_H_
#define GRAPHICSENGINEBINDINGDELEGATE_H_

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
namespace logger
{
class ILogger;
}

class GameEngine;

class GraphicsEngineBindingDelegate
{
public:
	GraphicsEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine);
	~GraphicsEngineBindingDelegate() = default;

	void bind();
	
private:
	logger::ILogger* logger_;
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	graphics::IGraphicsEngine* graphicsEngine_;
};

}

#endif /* GRAPHICSENGINEBINDINGDELEGATE_H_ */

