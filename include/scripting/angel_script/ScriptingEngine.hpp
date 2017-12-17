#ifndef SCRIPTINGENGINE_H_
#define SCRIPTINGENGINE_H_

#include <vector>

#include "scripting/IScriptingEngine.hpp"

#include "scripting/angel_script/scriptbuilder/scriptbuilder.h"

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
	ScriptHandle scriptHandle;
	std::string className;
	asIScriptObject* object;
};

class ScriptingEngine : public IScriptingEngine
{
public:
	ScriptingEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
	virtual ~ScriptingEngine();
	
	virtual void run(const std::string& filename, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void run(const std::string& filename, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	virtual void execute(const std::string& scriptData, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	virtual ExecutionContextHandle createExecutionContext() override;
	virtual void destroyExecutionContext(const ExecutionContextHandle& executionContextHandle) override;
	
	virtual void releaseScriptObject(const ScriptObjectHandle& scriptObjectHandle) override;
	virtual void releaseAllScriptObjects() override;
	
	virtual void releaseScriptFunction(const ScriptFunctionHandle& scriptFunctionHandle) override;
	virtual void releaseAllScriptFunctions() override;
	
	virtual void registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall = nullptr) override;
	virtual void registerGlobalProperty(const std::string& declaration, void* pointer) override;
	
	virtual void registerClass(const std::string& name) override;
	virtual void registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
		const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer) override;
		
	virtual void registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer) override;
	virtual void registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer) override;
	virtual void registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer) override;
		
	virtual void registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer) override;
	
	virtual void registerFunctionDefinition(const std::string& name) override;
	
	virtual void registerInterface(const std::string& name) override;
	virtual void registerInterfaceMethod(const std::string& name, const std::string& declaration) override;
	
	virtual void registerEnum(const std::string& type) override;
	virtual void registerEnumValue(const std::string& type, const std::string& name, const int64 value) override;
	
	virtual ScriptHandle loadScript(const std::string& filename, const std::string& name = std::string()) override;
	virtual void destroyScript(const std::string& name) override;
	virtual void destroyScript(const ScriptHandle& scriptHandle) override;
	virtual void destroyAllScripts() override;
	
	virtual ScriptFunctionHandle getScriptFunction(const ScriptHandle& scriptHandle, const std::string& function) override;
	virtual ScriptObjectFunctionHandle getScriptObjectFunction(const ScriptObjectHandle& scriptObjectHandle, const std::string& function) override;
	
	// More 'advanced' functions for angel script
	virtual void registerObjectType(const std::string& obj, const int32 byteSize, asDWORD flags) override;
	virtual void registerObjectMethod(const std::string& obj, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv, void* auxiliary = nullptr) override;
	virtual void registerObjectProperty(const std::string& obj, const std::string& declaration, int32 byteOffset) override;
	virtual void registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) override;

	virtual void MessageCallback(const asSMessageInfo* msg, void* param) override;
	static void print(const std::string& msg);
	static void println(const std::string& msg);

private:
	ScriptingEngine(const ScriptingEngine& other);
		
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
	handles::HandleVector<ScriptModuleData, ScriptHandle> moduleData_;
	
	asIScriptModule* getModule(const ScriptObjectHandle& scriptObjectHandle) const;
	asIScriptFunction* getMethod(const ScriptObjectHandle& scriptObjectHandle, const std::string& function) const;
	asITypeInfo* getType(const ScriptObjectHandle& scriptObjectHandle) const;
	
	asIScriptContext* getContext(const ExecutionContextHandle& executionContextHandle) const;
	asIScriptModule* createModuleFromScript(const std::string& moduleName, const std::string& scriptData) const;
	void destroyModule(const std::string& moduleName);
	
	void setArguments(asIScriptContext* context, ParameterList& arguments) const;
	
	void callFunction(asIScriptContext* context, asIScriptFunction* function, asIScriptObject* object) const;
	void callFunction(asIScriptContext* context, asIScriptFunction* function, asIScriptObject* object, ParameterList& arguments) const;
	void callFunction(asIScriptContext* context, asIScriptFunction* function) const;
	void callFunction(asIScriptContext* context, asIScriptFunction* function, ParameterList& arguments) const;
	void callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function) const;
	void callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function, ParameterList& arguments) const;
	void callFunction(asIScriptContext* context, const ScriptFunctionHandle& scriptFunctionHandle) const;
	void callFunction(asIScriptContext* context, const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments) const;
	void callFunction(asIScriptContext* context, const ScriptHandle& scriptHandle, const std::string& function) const;
	void callFunction(asIScriptContext* context, const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments) const;
	void callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function) const;
	void callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments) const;
	void callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle) const;
	void callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments) const;
	
	void assertNoAngelscriptError(const int32 returnCode) const;
	
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
