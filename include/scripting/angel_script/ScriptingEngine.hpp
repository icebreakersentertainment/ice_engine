#ifndef SCRIPTINGENGINE_H_
#define SCRIPTINGENGINE_H_

#include "scripting/IScriptingEngine.hpp"

#include "scripting/angel_script/scriptbuilder/scriptbuilder.h"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace hercules
{
namespace scripting
{
namespace angel_script
{

class ScriptingEngine : public IScriptingEngine
{
public:
	ScriptingEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger);
	virtual ~ScriptingEngine();
	
	/* AngelScript methods that need to be exposed */		
	virtual bool registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall = nullptr) override;
	virtual void registerGlobalProperty(const std::string& declaration, void* pointer) override;
	
	virtual bool registerClass(const std::string& name) override;
	virtual bool registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
		const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer) override;
		
	virtual bool registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer) override;
	virtual bool registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer) override;
	virtual bool registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer) override;
		
	virtual bool registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer) override;
	
	virtual bool loadScript(const std::string& name, const std::string& filename) override;
	virtual bool loadScripts() override;
	virtual bool loadScripts(const std::string& directory) override;
	virtual void unloadScripts() override;
	
	virtual int32 discardModule(const std::string& name) override;
	
	virtual AsObject* createAsObject(const std::string& moduleName, const std::string& className) override;
	
	// More 'advanced' functions for angel script
	virtual int32 registerObjectType(const std::string& obj, int32 byteSize, asDWORD flags) override;
	virtual int32 registerObjectMethod(const std::string& obj, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) override;
	virtual int32 registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) override;

	virtual void MessageCallback(const asSMessageInfo* msg, void* param) override;
	static void print(const std::string& msg);
	static void println(const std::string& msg);

	bool initContext(const std::string& function, const std::string& module) override;
	virtual int32 setArgDWord(asUINT arg, asDWORD value) override;
	virtual int32 setArgQWord(asUINT arg, asQWORD value) override;
	virtual int32 setArgFloat(asUINT arg, float32 value) override;
	virtual int32 setArgDouble(asUINT arg, float64 value) override;
	virtual int32 setArgAddress(asUINT arg, void* addr) override;
	virtual int32 setArgByte(asUINT arg, asBYTE value) override;
	virtual int32 setArgObject(asUINT arg, void* obj) override;
	virtual int32 setArgWord(asUINT arg, asWORD value) override;
	virtual int32 run() override;
	
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
	ScriptingEngine(const ScriptingEngine& other);
		
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	void initialize();
	void destroy();
	//int32 loadScript(const std::string& script);

	asIScriptEngine* engine_;
	CScriptBuilder* builder_;
	asIScriptContext* ctx_;
	
	int32 startNewModule(const std::string& module);
	int32 addScript(const std::string& script);
	int32 buildModule();
	int32 discardModules();
	
};

}
}
}

#endif /* SCRIPTINGENGINE_H_ */
