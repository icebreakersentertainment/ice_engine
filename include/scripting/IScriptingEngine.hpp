#ifndef ISCRIPTINGENGINE_H_
#define ISCRIPTINGENGINE_H_

#include <string>
#include <functional>

#include <angelscript/angelscript.h>

#include "scripting/angel_script/AsObject.hpp"

#include "scripting/ExecutionContextHandle.hpp"
#include "scripting/ScriptHandle.hpp"
#include "scripting/ScriptObjectHandle.hpp"
#include "scripting/Variant.hpp"

#include "Types.hpp"

namespace hercules
{
namespace scripting
{

class IScriptingEngine
{
public:
	virtual ~IScriptingEngine()
	{
	}
	;
	
	virtual void run(const std::string& filename, const std::string& function = std::string("void main()"), const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle()) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function = std::string("void main()"), const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	virtual void execute(const std::string& scriptData, const std::string& function, std::function<void(void*)> getReturnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) = 0;
	
	virtual ExecutionContextHandle createExecutionContext() = 0;
	
	virtual void registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall = nullptr) = 0;
	virtual void registerGlobalProperty(const std::string& declaration, void* pointer) = 0;
	
	virtual void registerClass(const std::string& name) = 0;
	virtual void registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
		const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer) = 0;
		
	virtual void registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer) = 0;
	virtual void registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer) = 0;
	virtual void registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer) = 0;
		
	virtual void registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer) = 0;
	
	virtual void loadScript(const std::string& name, const std::string& filename) = 0;
	virtual void loadScripts() = 0;
	virtual void loadScripts(const std::string& directory) = 0;
	virtual void unloadScripts() = 0;
	
	virtual void discardModule(const std::string& name) = 0;
	
	virtual void runScript(const std::string& filename, const std::string& function = std::string("void main()")) = 0;
	
	virtual angel_script::AsObject* createAsObject(const std::string& moduleName, const std::string& className) = 0;
	
	// More 'advanced' functions for angel script
	virtual void registerObjectType(const std::string& obj, const int32 byteSize, asDWORD flags) = 0;
	virtual void registerObjectMethod(const std::string& obj, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) = 0;
	virtual void registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) = 0;

	virtual void MessageCallback(const asSMessageInfo* msg, void* param) = 0;

	virtual void initContext(const std::string& function, const std::string& module) = 0;
	virtual void setArgDWord(asUINT arg, asDWORD value) = 0;
	virtual void setArgQWord(asUINT arg, asQWORD value) = 0;
	virtual void setArgFloat(asUINT arg, const float32 value) = 0;
	virtual void setArgDouble(asUINT arg, const float64 value) = 0;
	virtual void setArgAddress(asUINT arg, void* addr) = 0;
	virtual void setArgByte(asUINT arg, asBYTE value) = 0;
	virtual void setArgObject(asUINT arg, void* obj) = 0;
	virtual void setArgWord(asUINT arg, asWORD value) = 0;
	virtual void run() = 0;
	
};

}

}

#endif /* ISCRIPTINGENGINE_H_ */
