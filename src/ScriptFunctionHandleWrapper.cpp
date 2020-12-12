#include "ScriptFunctionHandleWrapper.hpp"

namespace ice_engine
{

void ScriptFunctionHandleWrapperDeleter(std::pair<scripting::IScriptingEngine*, scripting::ScriptFunctionHandle>* data)
{
	data->first->releaseScriptFunction(data->second);
}

ScriptFunctionHandleWrapper::ScriptFunctionHandleWrapper(scripting::IScriptingEngine* scriptingEngine, scripting::ScriptFunctionHandle scriptFunctionHandle)
	:
	data_(new std::pair<scripting::IScriptingEngine*, scripting::ScriptFunctionHandle>(scriptingEngine, scriptFunctionHandle), ScriptFunctionHandleWrapperDeleter)
{
	
}

ScriptFunctionHandleWrapper::~ScriptFunctionHandleWrapper()
{

}

scripting::ScriptFunctionHandle ScriptFunctionHandleWrapper::get() const
{
	return data_->second;
}

}
