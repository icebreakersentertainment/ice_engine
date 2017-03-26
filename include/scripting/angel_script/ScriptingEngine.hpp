#ifndef SCRIPTINGENGINE_H_
#define SCRIPTINGENGINE_H_

#include <vector>

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
	
	virtual void run(const std::string& filename, const std::string& function = std::string("void main()"), const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function = std::string("void main()"), const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	virtual void execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser = [](void*){}, const ExecutionContextHandle& executionContextHandle = ExecutionContextHandle(0)) override;
	
	virtual ExecutionContextHandle createExecutionContext() override;
		
	virtual void registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall = nullptr) override;
	virtual void registerGlobalProperty(const std::string& declaration, void* pointer) override;
	
	virtual void registerClass(const std::string& name) override;
	virtual void registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
		const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer) override;
		
	virtual void registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer) override;
	virtual void registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer) override;
	virtual void registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer) override;
		
	virtual void registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer) override;
	
	virtual void loadScript(const std::string& name, const std::string& filename) override;
	virtual void loadScripts() override;
	virtual void loadScripts(const std::string& directory) override;
	virtual void unloadScripts() override;
	
	virtual void runScript(const std::string& filename, const std::string& function = std::string("void main()")) override;
	
	virtual void discardModule(const std::string& name) override;
	
	virtual AsObject* createAsObject(const std::string& moduleName, const std::string& className) override;
	
	// More 'advanced' functions for angel script
	virtual void registerObjectType(const std::string& obj, const int32 byteSize, asDWORD flags) override;
	virtual void registerObjectMethod(const std::string& obj, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) override;
	virtual void registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour, const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv) override;

	virtual void MessageCallback(const asSMessageInfo* msg, void* param) override;
	static void print(const std::string& msg);
	static void println(const std::string& msg);

	virtual void initContext(const std::string& function, const std::string& module) override;
	virtual void setArgDWord(asUINT arg, asDWORD value) override;
	virtual void setArgQWord(asUINT arg, asQWORD value) override;
	virtual void setArgFloat(asUINT arg, const float32 value) override;
	virtual void setArgDouble(asUINT arg, const float64 value) override;
	virtual void setArgAddress(asUINT arg, void* addr) override;
	virtual void setArgByte(asUINT arg, asBYTE value) override;
	virtual void setArgObject(asUINT arg, void* obj) override;
	virtual void setArgWord(asUINT arg, asWORD value) override;
	virtual void run() override;
	
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

	void releaseAngelscriptIScriptEngine(asIScriptEngine* engine);
	void releaseAngelscriptIScriptContext(asIScriptContext* context);

	asIScriptContext* ctx_;
	std::unique_ptr<CScriptBuilder> builder_;
	asIScriptEngine* engine_;
	std::vector< asIScriptContext* > contexts_;
	asIScriptModule* defaultModule_;
	
	void assertNoAngelscriptError(const int32 returnCode);
	
	void startNewModule(const std::string& module);
	void addScript(const std::string& script);
	void buildModule();
	void discardModules();
	
	asIScriptFunction* getFunctionByDecl(const std::string& function, asIScriptModule* module = nullptr);
	
	static const std::string ONE_TIME_RUN_SCRIPT_MODULE_NAME;
	static const uint32 MAX_EXECUTION_CONTEXTS = 32;
};

}
}
}

#endif /* SCRIPTINGENGINE_H_ */
