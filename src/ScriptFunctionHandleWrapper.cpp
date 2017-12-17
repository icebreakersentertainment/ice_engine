#include <iostream>

#include "ScriptFunctionHandleWrapper.hpp"

namespace ice_engine
{

ScriptFunctionHandleWrapper::ScriptFunctionHandleWrapper(scripting::IScriptingEngine* scriptingEngine, scripting::ScriptFunctionHandle scriptFunctionHandle)
	:
	scriptingEngine_(scriptingEngine), scriptFunctionHandle_(scriptFunctionHandle)
{
	
}

ScriptFunctionHandleWrapper::~ScriptFunctionHandleWrapper()
{
	std::cout << "destroyed" << std::endl;
	scriptingEngine_->releaseScriptFunction(scriptFunctionHandle_);
}

scripting::ScriptFunctionHandle ScriptFunctionHandleWrapper::get() const
{
	return scriptFunctionHandle_;
}

}
