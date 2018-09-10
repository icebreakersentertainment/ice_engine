#ifndef SCRIPT_FUNCTION_HANDLE_WRAPPER_H_
#define SCRIPT_FUNCTION_HANDLE_WRAPPER_H_

#include <memory>
#include <utility>

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
	std::shared_ptr<std::pair<scripting::IScriptingEngine*, scripting::ScriptFunctionHandle>> data_;
};

}

#endif /* SCRIPT_FUNCTION_HANDLE_WRAPPER_H_ */
