#ifndef ISCRIPTINGENGINE_H_
#define ISCRIPTINGENGINE_H_

#include <string>

#include <angelscript/angelscript.h>

#include "scripting/angel_script/AsObject.hpp"

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
	
	/* AngelScript methods that need to be exposed */		
	virtual bool registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall = nullptr) = 0;
	virtual void registerGlobalProperty(const std::string& declaration, void* pointer) = 0;
	
	virtual bool registerClass(const std::string& name) = 0;
	virtual bool registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
		const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer) = 0;
		
	virtual bool registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer) = 0;
	virtual bool registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer) = 0;
	virtual bool registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer) = 0;
		
	virtual bool registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer) = 0;
	
	virtual bool loadScript(const std::string& name, const std::string& filename) = 0;
	virtual bool loadScripts() = 0;
	virtual bool loadScripts(const std::string& directory) = 0;
	virtual void unloadScripts() = 0;
	
	virtual int32 discardModule(const std::string& name) = 0;
	
	virtual angel_script::AsObject* createAsObject(const std::string& moduleName, const std::string& className) = 0;
	
	// More 'advanced' functions for angel script
	virtual int32 registerObjectType(const std::string& obj, int32 byteSize, asDWORD flags) = 0;
	virtual int32 registerObjectMethod(const std::string& obj, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) = 0;
	virtual int32 registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) = 0;

	virtual void MessageCallback(const asSMessageInfo* msg, void* param) = 0;

	virtual bool initContext(const std::string& function, const std::string& module) = 0;
	virtual int32 setArgDWord(asUINT arg, asDWORD value) = 0;
	virtual int32 setArgQWord(asUINT arg, asQWORD value) = 0;
	virtual int32 setArgFloat(asUINT arg, float32 value) = 0;
	virtual int32 setArgDouble(asUINT arg, float64 value) = 0;
	virtual int32 setArgAddress(asUINT arg, void* addr) = 0;
	virtual int32 setArgByte(asUINT arg, asBYTE value) = 0;
	virtual int32 setArgObject(asUINT arg, void* obj) = 0;
	virtual int32 setArgWord(asUINT arg, asWORD value) = 0;
	virtual int32 run() = 0;
	
};

}

}

#endif /* ISCRIPTINGENGINE_H_ */
