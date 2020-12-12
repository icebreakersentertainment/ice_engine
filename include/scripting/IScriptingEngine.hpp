#ifndef ISCRIPTINGENGINE_H_
#define ISCRIPTINGENGINE_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

#include <angelscript/angelscript.h>

#include "scripting/angel_script/autowrapper/aswrappedcall.h"

#include "scripting/ModuleHandle.hpp"
#include "scripting/ExecutionContextHandle.hpp"
#include "scripting/ScriptObjectHandle.hpp"
#include "scripting/ScriptFunctionHandle.hpp"
#include "scripting/ScriptObjectFunctionHandle.hpp"
#include "scripting/ParameterList.hpp"
#include "scripting/IScriptingEngineDebugger.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace scripting
{

class IScriptingEngine
{
public:
	virtual ~IScriptingEngine() = default;
	
	virtual void run(const std::string& filename, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	
	virtual void execute(const std::string& scriptData, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint32& returnaszValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	
	virtual ExecutionContextHandle createExecutionContext() = 0;
	virtual void destroyExecutionContext(const ExecutionContextHandle& executionContextHandle) = 0;
	
	virtual std::string getScriptObjectName(const ScriptObjectHandle& scriptObjectHandle) const = 0;

	virtual ScriptObjectHandle createUninitializedScriptObject(const ModuleHandle& moduleHandle, const std::string& name) = 0;

	virtual ModuleHandle createModule(const std::string& name, const std::vector<std::string>& scriptData, const std::unordered_map<std::string, std::string>& includeOverrides = {}) = 0;
	virtual ModuleHandle getModule(const std::string& name) const = 0;
	virtual void destroyModule(const ModuleHandle& moduleHandle) = 0;
	virtual void destroyAllModules() = 0;

	virtual void releaseScriptObject(const ScriptObjectHandle& scriptObjectHandle) = 0;
	virtual void releaseAllScriptObjects() = 0;

	virtual void releaseScriptObjectFunction(const ScriptObjectFunctionHandle& scriptObjectFunctionHandle) = 0;
	virtual void releaseAllScriptObjectFunctions() = 0;
	
	virtual void releaseScriptFunction(const ScriptFunctionHandle& scriptFunctionHandle) = 0;
	virtual void releaseAllScriptFunctions() = 0;

    virtual void tick(const float32 delta) = 0;
	
	virtual void registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall = nullptr) = 0;
	virtual void registerGlobalProperty(const std::string& declaration, void* pointer) = 0;
	
	virtual void registerClass(const std::string& name) = 0;
	virtual void registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
		const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer) = 0;
		
	virtual void registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer) = 0;
	virtual void registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer) = 0;
	virtual void registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer) = 0;
		
	virtual void registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer) = 0;
	
	virtual void registerFunctionDefinition(const std::string& name) = 0;
	
	virtual void registerInterface(const std::string& name) = 0;
	virtual void registerInterfaceMethod(const std::string& name, const std::string& declaration) = 0;
	
	virtual void registerEnum(const std::string& type) = 0;
	virtual void registerEnumValue(const std::string& type, const std::string& name, const int64 value) = 0;
	
	virtual ScriptFunctionHandle getScriptFunction(const ModuleHandle& moduleHandle, const std::string& function) = 0;
	virtual ScriptObjectFunctionHandle getScriptObjectFunction(const ScriptObjectHandle& scriptObjectHandle, const std::string& function) = 0;
	
	// More 'advanced' functions for angel script
	virtual void registerObjectType(const std::string& obj, const int32 byteSize, asDWORD flags) = 0;
	virtual void registerObjectMethod(const std::string& obj, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv, void* auxiliary = nullptr) = 0;
	virtual void registerObjectProperty(const std::string& obj, const std::string& declaration, int32 byteOffset) = 0;
	virtual void registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) = 0;

	virtual IScriptingEngineDebugger* debugger() = 0;

	virtual void MessageCallback(const asSMessageInfo* msg, void* param) = 0;

	virtual void testPrintCallstack() = 0;

};

}

}

#endif /* ISCRIPTINGENGINE_H_ */
