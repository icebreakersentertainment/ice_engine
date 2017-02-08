#ifndef ANGELSCRIPT_H_
#define ANGELSCRIPT_H_

#include <angelscript/angelscript.h>

#include "scriptbuilder/scriptbuilder.h"

#include "AScriptModule.h"
#include "AsObject.h"

#include "logger/ILogger.hpp"
#include "Types.hpp"

namespace hercules
{
namespace as_wrapper
{
	
class AngelScript
{
public:
	AngelScript(logger::ILogger* logger);
	virtual ~AngelScript();
	
	/* AngelScript methods that need to be exposed */		
	bool registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall = nullptr);
	void registerGlobalProperty(const std::string& declaration, void* pointer);
	
	bool registerClass(const std::string& name);
	bool registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
		const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer);
		
	bool registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer);
	bool registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer);
	bool registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer);
		
	bool registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer);
	
	bool loadScript(const std::string& name, const std::string& filename);
	bool loadScripts();
	bool loadScripts(const std::string& directory);
	void unloadScripts();
	
	AScriptModule* getScript(const std::string& name);
	int32 discardModule(const std::string& name);
	
	AsObject* createAsObject(const std::string& moduleName, const std::string& className);

	// More 'advanced' functions for angel script
	int32 registerObjectType(const std::string& obj, int32 byteSize, asDWORD flags);
	int32 registerObjectMethod(const std::string& obj, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv);
	int32 registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv);

	void MessageCallback(const asSMessageInfo* msg, void* param);
	static void print(const std::string& msg);
	static void println(const std::string& msg);

	bool initContext(const std::string& function, const std::string& module);
	int32 setArgDWord(asUINT arg, asDWORD value);
	int32 setArgQWord(asUINT arg, asQWORD value);
	int32 setArgFloat(asUINT arg, float32 value);
	int32 setArgDouble(asUINT arg, float64 value);
	int32 setArgAddress(asUINT arg, void* addr);
	int32 setArgByte(asUINT arg, asBYTE value);
	int32 setArgObject(asUINT arg, void* obj);
	int32 setArgWord(asUINT arg, asWORD value);
	int32 run();
	asDWORD getReturnDWord();
	asQWORD getReturnQWord();
	float32 getReturnFloat();
	float64 getReturnDouble();
	void* getReturnAddress();
	asBYTE getReturnByte();
	void* getReturnObject();
	asWORD getReturnWord();
	void releaseContext();

private:
	void initialize();
	void destroy();
	//int32 loadScript(const std::string& script);

	asIScriptEngine* engine_;
	CScriptBuilder* builder_;
	asIScriptContext* ctx_;
	
	logger::ILogger* logger_;

	int32 startNewModule(const std::string& module);
	int32 addScript(const std::string& script);
	int32 buildModule();
	int32 discardModules();
};

}
}

#endif /* ANGELSCRIPT_H_ */
