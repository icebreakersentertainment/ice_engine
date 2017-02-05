#ifndef ANGELSCRIPT_H_
#define ANGELSCRIPT_H_

#include <angelscript/angelscript.h>

#include "scriptbuilder/scriptbuilder.h"

#include "AScriptModule.h"
#include "AsObject.h"

#include "../utilities/Macros.hpp"

namespace hercules
{
namespace as_wrapper
{
	
class AngelScript
{
public:
	AngelScript();
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
	int discardModule(const std::string& name);
	
	AsObject* createAsObject(const std::string& moduleName, const std::string& className);

	// More 'advanced' functions for angel script
	int registerObjectType(const std::string& obj, int byteSize, asDWORD flags);
	int registerObjectMethod(const std::string& obj, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv);
	int registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv);

	void MessageCallback(const asSMessageInfo* msg, void* param);
	static void print(const std::string& msg);
	static void println(const std::string& msg);

	bool initContext(const std::string& function, const std::string& module);
	int setArgDWord(asUINT arg, asDWORD value);
	int setArgQWord(asUINT arg, asQWORD value);
	int setArgFloat(asUINT arg, float value);
	int setArgDouble(asUINT arg, double value);
	int setArgAddress(asUINT arg, void* addr);
	int setArgByte(asUINT arg, asBYTE value);
	int setArgObject(asUINT arg, void* obj);
	int setArgWord(asUINT arg, asWORD value);
	int run();
	asDWORD getReturnDWord();
	asQWORD getReturnQWord();
	float getReturnFloat();
	double getReturnDouble();
	void* getReturnAddress();
	asBYTE getReturnByte();
	void* getReturnObject();
	asWORD getReturnWord();
	void releaseContext();

private:
	void initialize();
	void destroy();
	//int loadScript(const std::string& script);

	asIScriptEngine* engine_;
	CScriptBuilder* builder_;
	asIScriptContext* ctx_;

	int startNewModule(const std::string& module);
	int addScript(const std::string& script);
	int buildModule();
	int discardModules();
};

}
}

#endif /* ANGELSCRIPT_H_ */
