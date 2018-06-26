#ifndef SCRIPTINGENGINEBINDINGDELEGATE_H_
#define SCRIPTINGENGINEBINDINGDELEGATE_H_

namespace ice_engine
{

namespace scripting
{
class IScriptingEngine;
}
namespace logger
{
class ILogger;
}

class GameEngine;

class ScriptingEngineBindingDelegate
{
public:
	ScriptingEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine);
	virtual ~ScriptingEngineBindingDelegate();

	void bind();
	
private:
	logger::ILogger* logger_;
	scripting::IScriptingEngine* scriptingEngine_;
	
};

}

#endif /* SCRIPTINGENGINEBINDINGDELEGATE_H_ */

