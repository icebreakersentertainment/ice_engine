#ifndef SCRIPTINGENGINE_H_
#define SCRIPTINGENGINE_H_

#include <vector>

#include "scripting/IScriptingEngine.hpp"

#include "scripting/angel_script/AngelscriptDebugger.hpp"

#include "scripting/angel_script/scriptbuilder/scriptbuilder.h"
#include "scripting/angel_script/scripthandle/scripthandle.h"

#include "handles/HandleVector.hpp"
#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{
namespace scripting
{
namespace angel_script
{

namespace
{
struct ScriptContextData
{
    asIScriptContext* context;
};

struct ScriptModuleData
{
    asIScriptModule* module;
};

struct ScriptObjectData
{
    ModuleHandle moduleHandle;
    std::string className;
    asIScriptObject* object;
};
}

class ScriptingEngine : public IScriptingEngine
{
public:
	ScriptingEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
    ScriptingEngine(const ScriptingEngine& other) = delete;
	~ScriptingEngine() override;
	
	void run(const std::string& filename, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	void execute(const std::string& scriptData, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	void execute(const ModuleHandle& moduleHandle, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	ExecutionContextHandle createExecutionContext() override;
	void destroyExecutionContext(const ExecutionContextHandle& executionContextHandle) override;
	
	std::string getScriptObjectName(const ScriptObjectHandle& scriptObjectHandle) const override;

	ScriptObjectHandle createUninitializedScriptObject(const ModuleHandle& moduleHandle, const std::string& name) override;

	ModuleHandle createModule(const std::string& name, const std::vector<std::string>& scriptData, const std::unordered_map<std::string, std::string>& includeOverrides = {}) override;
	ModuleHandle getModule(const std::string& name) const override;
	void destroyModule(const ModuleHandle& moduleHandle) override;
	void destroyAllModules() override;

	void releaseScriptObject(const ScriptObjectHandle& scriptObjectHandle) override;
	void releaseAllScriptObjects() override;
	
	void releaseScriptObjectFunction(const ScriptObjectFunctionHandle& scriptObjectFunctionHandle) override;
	void releaseAllScriptObjectFunctions() override;

	void releaseScriptFunction(const ScriptFunctionHandle& scriptFunctionHandle) override;
	void releaseAllScriptFunctions() override;

    void tick(const float32 delta) override;

    void registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall = nullptr) override;
	void registerGlobalProperty(const std::string& declaration, void* pointer) override;
	
	void registerClass(const std::string& name) override;
	void registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
		const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer) override;
		
	void registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer) override;
	void registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer) override;
	void registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer) override;
		
	void registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer) override;
	
	void registerFunctionDefinition(const std::string& name) override;
	
	void registerInterface(const std::string& name) override;
	void registerInterfaceMethod(const std::string& name, const std::string& declaration) override;
	
	void registerEnum(const std::string& type) override;
	void registerEnumValue(const std::string& type, const std::string& name, const int64 value) override;
	
	ScriptFunctionHandle getScriptFunction(const ModuleHandle& moduleHandle, const std::string& function) override;
	ScriptObjectFunctionHandle getScriptObjectFunction(const ScriptObjectHandle& scriptObjectHandle, const std::string& function) override;
	
	// More 'advanced' functions for angel script
	void registerObjectType(const std::string& obj, const int32 byteSize, asDWORD flags) override;
	void registerObjectMethod(const std::string& obj, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv, void* auxiliary = nullptr) override;
	void registerObjectProperty(const std::string& obj, const std::string& declaration, int32 byteOffset) override;
	void registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) override;

    IScriptingEngineDebugger* debugger() override;

	void MessageCallback(const asSMessageInfo* msg, void* param) override;

    void testPrintCallstack() override;

    static void print(const std::string& msg);
	static void println(const std::string& msg);

private:
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	void initialize();
	void destroy();

	void releaseAngelscriptIScriptEngine(asIScriptEngine* engine);
	void releaseAngelscriptIScriptContext(asIScriptContext* context);

	asIScriptContext* ctx_;
	std::unique_ptr<CScriptBuilder> builder_;
	asIScriptEngine* engine_;
	handles::HandleVector<ScriptContextData, ExecutionContextHandle> contextData_;
	handles::HandleVector<ScriptModuleData, ModuleHandle> moduleData_;

    std::unique_ptr<AngelscriptDebugger> debugger_;
	
	asIScriptModule* getModule(const ScriptObjectHandle& scriptObjectHandle) const;
	asIScriptFunction* getMethod(const ScriptObjectHandle& scriptObjectHandle, const std::string& function) const;
	asITypeInfo* getType(const ScriptObjectHandle& scriptObjectHandle) const;
	
	asIScriptContext* getContext(const ExecutionContextHandle& executionContextHandle) const;
	asIScriptModule* createModuleFromScript(const std::string& moduleName, const std::string& scriptData);
	asIScriptModule* createModuleFromScripts(const std::string& moduleName, const std::vector<std::string>& scriptData, const std::unordered_map<std::string, std::string>& includeOverrides = {});
	void destroyModule(const std::string& moduleName);

	CScriptHandle createScriptObjectReturnAsScriptHandle(const ModuleHandle& moduleHandle, const std::string& objectName, const std::string& factoryName, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0));
	asIScriptObject* createScriptObject(const ModuleHandle& moduleHandle, const std::string& objectName, const std::string& factoryName, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0));

	void setArguments(asIScriptContext* context, ParameterList& arguments) const;

    asIScriptObject* callFunctionWithReturnValue(asIScriptContext* context, asIScriptFunction* function);

	void callFunction(asIScriptContext* context, asIScriptFunction* function, asIScriptObject* object);
	void callFunction(asIScriptContext* context, asIScriptFunction* function, asIScriptObject* object, ParameterList& arguments);
	void callFunction(asIScriptContext* context, asIScriptFunction* function);
	void callFunction(asIScriptContext* context, asIScriptFunction* function, ParameterList& arguments);
	void callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function);
	void callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function, ParameterList& arguments);
	void callFunction(asIScriptContext* context, const ScriptFunctionHandle& scriptFunctionHandle);
	void callFunction(asIScriptContext* context, const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments);
	void callFunction(asIScriptContext* context, const ModuleHandle& moduleHandle, const std::string& function);
	void callFunction(asIScriptContext* context, const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments);
	void callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function);
	void callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments);
	void callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle);
	void callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments);

	asIScriptFunction* getFunctionByDecl(const std::string& function, const asIScriptModule* module) const;
	asIScriptFunction* getFunctionByDecl(const std::string& function, const asIScriptObject* object) const;
	
	void discardModule(const std::string& name);
	
	static const std::string ONE_TIME_RUN_SCRIPT_MODULE_NAME;
	static const uint32 MAX_EXECUTION_CONTEXTS = 32;
};

}
}
}

#endif /* SCRIPTINGENGINE_H_ */
