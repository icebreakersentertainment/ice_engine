#ifndef SCRIPT_FUNCTION_HANDLE_WRAPPER_H_
#define SCRIPT_FUNCTION_HANDLE_WRAPPER_H_

#include "scripting/IScriptingEngine.hpp"

namespace ice_engine
{

class ScriptFunctionHandleWrapper
{
public:
	ScriptFunctionHandleWrapper(scripting::IScriptingEngine* scriptingEngine, scripting::ScriptFunctionHandle scriptFunctionHandle);
	virtual ~ScriptFunctionHandleWrapper();
	
	scripting::ScriptFunctionHandle get() const;
	
private:
	scripting::IScriptingEngine* scriptingEngine_;
	scripting::ScriptFunctionHandle scriptFunctionHandle_;
};

}

#endif /* SCRIPT_FUNCTION_HANDLE_WRAPPER_H_ */
