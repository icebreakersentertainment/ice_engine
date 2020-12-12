#include <string>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "ScriptingEngineBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

namespace ice_engine
{

ScriptingEngineBindingDelegate::ScriptingEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine)
{
}

ScriptingEngineBindingDelegate::~ScriptingEngineBindingDelegate()
{
}

void ScriptingEngineBindingDelegate::bind()
{
	registerPointerHandleBindings<scripting::ScriptObjectHandle>(scriptingEngine_, "ScriptObjectHandle", "IScriptObject");
//	scriptingEngine_->registerObjectType("ScriptObjectHandle", sizeof(scripting::ScriptObjectHandle), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asOBJ_POD | asGetTypeTraits<scripting::ScriptObjectHandle>());

    // IScriptingEngine
    scriptingEngine_->registerObjectType("IScriptingEngine", 0, asOBJ_REF | asOBJ_NOCOUNT);
    scriptingEngine_->registerGlobalProperty("IScriptingEngine scripting", scriptingEngine_);

	// Future bindings
	scriptingEngine_->registerEnum("future_status");
	scriptingEngine_->registerEnumValue("future_status", "deferred", static_cast<int64>(std::future_status::deferred));
	scriptingEngine_->registerEnumValue("future_status", "ready", static_cast<int64>(std::future_status::ready));
	scriptingEngine_->registerEnumValue("future_status", "timeout", static_cast<int64>(std::future_status::timeout));

    scriptingEngine_->registerEnum("DebugAction");
    scriptingEngine_->registerEnumValue("DebugAction", "CONTINUE", scripting::DebugAction::CONTINUE);
    scriptingEngine_->registerEnumValue("DebugAction", "STEP_INTO", scripting::DebugAction::STEP_INTO);
    scriptingEngine_->registerEnumValue("DebugAction", "STEP_OUT", scripting::DebugAction::STEP_OUT);
    scriptingEngine_->registerEnumValue("DebugAction", "STEP_OVER", scripting::DebugAction::STEP_OVER);

	// Debugger
    scriptingEngine_->registerObjectType("IScriptingEngineDebugger", 0, asOBJ_REF | asOBJ_NOCOUNT);
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "void addBreakPoint(const string& in, const uint32)",
            asMETHOD(scripting::IScriptingEngineDebugger, addBreakPoint)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "void performAction(const DebugAction)",
            asMETHOD(scripting::IScriptingEngineDebugger, performAction)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "string filename() const",
            asMETHOD(scripting::IScriptingEngineDebugger, filename)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "string functionName() const",
            asMETHOD(scripting::IScriptingEngineDebugger, functionName)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "int32 line() const",
            asMETHOD(scripting::IScriptingEngineDebugger, line)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "string stacktrace()",
            asMETHOD(scripting::IScriptingEngineDebugger, stacktrace)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "string variables()",
            asMETHOD(scripting::IScriptingEngineDebugger, variables)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "bool enabled() const",
            asMETHOD(scripting::IScriptingEngineDebugger, enabled)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "void setEnabled(const bool enabled)",
            asMETHOD(scripting::IScriptingEngineDebugger, setEnabled)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "bool running() const",
            asMETHOD(scripting::IScriptingEngineDebugger, running)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngineDebugger",
            "void setRunning(const bool running)",
            asMETHOD(scripting::IScriptingEngineDebugger, setRunning)
    );
    scriptingEngine_->registerClassMethod(
            "IScriptingEngine",
            "IScriptingEngineDebugger@ debugger()",
            asMETHOD(scripting::IScriptingEngine, debugger)
    );

	// SceneThingy
	scriptingEngine_->registerInterface("SceneThingy");
	scriptingEngine_->registerInterfaceMethod("SceneThingy", "void preTick(const float)");
	scriptingEngine_->registerInterfaceMethod("SceneThingy", "void postTick(const float)");

    scriptingEngine_->registerGlobalFunction(
            "void testPrintCallstack()",
            asMETHOD(scripting::IScriptingEngine, testPrintCallstack),
            asCALL_THISCALL_ASGLOBAL,
            scriptingEngine_
    );
}
	
};
