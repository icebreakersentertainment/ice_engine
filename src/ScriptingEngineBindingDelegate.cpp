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

	// Future bindings
	scriptingEngine_->registerEnum("future_status");
	scriptingEngine_->registerEnumValue("future_status", "deferred", static_cast<int64>(std::future_status::deferred));
	scriptingEngine_->registerEnumValue("future_status", "ready", static_cast<int64>(std::future_status::ready));
	scriptingEngine_->registerEnumValue("future_status", "timeout", static_cast<int64>(std::future_status::timeout));
	
	// SceneThingy
	scriptingEngine_->registerInterface("SceneThingy");
	scriptingEngine_->registerInterfaceMethod("SceneThingy", "void preTick(const float)");
	scriptingEngine_->registerInterfaceMethod("SceneThingy", "void postTick(const float)");
}
	
};
