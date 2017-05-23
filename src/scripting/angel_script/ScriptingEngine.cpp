#include <iostream>
#include <stdio.h>

#include "scripting/angel_script/ScriptingEngine.hpp"

#include "scripting/angel_script/scripthelper/scripthelper.h"
#include "scripting/angel_script/scriptarray/scriptarray.h"
#include "scripting/angel_script/scriptstdstring/scriptstdstring.h"
//#include "scripting/angel_script/glm_bindings/Vec3.h"
#include "scripting/angel_script/scriptvector/scriptvector.hpp"
#include "scripting/angel_script/scriptglm/scriptglm.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/InvalidArgumentException.hpp"

namespace hercules
{
namespace scripting
{
namespace angel_script
{

const std::string ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME = std::string("ONE_TIME_RUN_SCRIPT_MODULE_NAME");

ScriptingEngine::ScriptingEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger)
{
	properties_ = properties;
	fileSystem_ = fileSystem;
	logger_ = logger;
	
	ctx_ = nullptr;
	
	initialize();
}

ScriptingEngine::ScriptingEngine(const ScriptingEngine& other)
{
}

ScriptingEngine::~ScriptingEngine()
{
	destroy();
}

void ScriptingEngine::assertNoAngelscriptError(const int32 returnCode) const
{
	if (returnCode < 0)
	{
		switch(returnCode)
		{
			case asERROR:
				throw Exception("ScriptEngine: Generic error.");
				break;
				
			case asINVALID_ARG:
				throw InvalidArgumentException("ScriptEngine: Argument was invalid.");
				break;
			
			case asNOT_SUPPORTED:
				throw Exception("ScriptEngine: Operation not supported.");
				break;
				
			case asNO_MODULE:
				throw Exception("ScriptEngine: Module not found.");
				break;
			
			case asINVALID_TYPE:
				throw Exception("ScriptEngine: The type specified is invalid.");
				break;
			
			case asNO_GLOBAL_VAR:
				throw Exception("ScriptEngine: No matching property was found.");
				break;
			
			case asINVALID_DECLARATION:
				throw Exception("ScriptEngine: The specified declaration is invalid.");
				break;
			
			case asINVALID_NAME:
				throw Exception("ScriptEngine: The name specified is invalid.");
				break;
			
			case asALREADY_REGISTERED:
				throw Exception("ScriptEngine: The specified type or name is already registered.");
				break;
			
			case asNAME_TAKEN:
				throw Exception("ScriptEngine: The specified name is already taken.");
				break;
			
			case asWRONG_CALLING_CONV:
				throw Exception("ScriptEngine: The specified calling convention is not valid or does not match the registered calling convention.");
				break;
			
			case asWRONG_CONFIG_GROUP:
				throw Exception("ScriptEngine: Wrong configuration group.");
				break;
			
			case asCONFIG_GROUP_IS_IN_USE:
				throw Exception("ScriptEngine: Configuration group already in use.");
				break;
			
			case asILLEGAL_BEHAVIOUR_FOR_TYPE:
				throw Exception("ScriptEngine: Illegal behaviour for type.");
				break;
			
			case asINVALID_OBJECT:
				throw Exception("ScriptEngine: The object does not specify a valid object type.");
				break;
			
			case asLOWER_ARRAY_DIMENSION_NOT_REGISTERED:
				throw Exception("ScriptEngine:  Array element must be a primitive or a registered type.");
				break;
			
			default:
				std::string str = std::string("ScriptEngine: Unknown error: ") + std::to_string(returnCode);
				throw Exception(str.c_str());
				break;
		}
	}
}

void ScriptingEngine::initialize()
{
	engine_ = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	
	// Set the message callback to receive information on errors in human readable form.
	int32 r = engine_->SetMessageCallback(asMETHOD(ScriptingEngine, MessageCallback), this, asCALL_THISCALL);
	assertNoAngelscriptError(r);

	/* ScriptingEngine doesn't have a built-in string type, as there is no definite standard
	 * string type for C++ applications. Every developer is free to register it's own string type.
	 * The SDK do however provide a standard add-on for registering a string type, so it's not
	 * necessary to implement the registration yourself if you don't want to.
	 */
	RegisterStdString(engine_);
	
	RegisterScriptArray(engine_, true);
	
	RegisterVectorBindings<int8>(engine_, "vectorInt8", "int8");
	RegisterVectorBindings<uint8>(engine_, "vectorUInt8", "uint8");
	RegisterVectorBindings<int16>(engine_, "vectorInt16", "int16");
	RegisterVectorBindings<uint16>(engine_, "vectorUInt16", "uint16");
	RegisterVectorBindings<int>(engine_, "vectorInt", "int");
	RegisterVectorBindings<int32>(engine_, "vectorInt32", "int32");
	RegisterVectorBindings<uint32>(engine_, "vectorUInt32", "uint32");
	RegisterVectorBindings<int64>(engine_, "vectorInt64", "int64");
	RegisterVectorBindings<uint64>(engine_, "vectorUInt64", "uint64");
	RegisterVectorBindings<float32>(engine_, "vectorFloat", "float");
	RegisterVectorBindings<float64>(engine_, "vectorDouble", "double");
	
	RegisterGlmBindings(engine_);
	
	RegisterVectorBindings<glm::vec2>(engine_, "vectorVec2", "vec2");
	RegisterVectorBindings<glm::vec3>(engine_, "vectorVec3", "vec3");
	RegisterVectorBindings<glm::vec4>(engine_, "vectorVec4", "vec4");
	RegisterVectorBindings<glm::ivec2>(engine_, "vectorIVec2", "ivec2");
	RegisterVectorBindings<glm::ivec3>(engine_, "vectorIVec3", "ivec3");
	RegisterVectorBindings<glm::ivec4>(engine_, "vectorIVec4", "ivec4");
	RegisterVectorBindings<glm::quat>(engine_, "vectorQuat", "quat");
	
	/* The CScriptBuilder helper is an add-on that does the loading/processing
	 * of a script file
	 */
	builder_ = std::make_unique<CScriptBuilder>();

	/**
	 * Setup the functions that are available to the scripts.
	 */
	// Register the function(s) that we want the scripts to call
	r = engine_->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(scripting::angel_script::ScriptingEngine::print), asCALL_CDECL);
	assertNoAngelscriptError(r);
	
	r = engine_->RegisterGlobalFunction("void println(const string &in)", asFUNCTION(scripting::angel_script::ScriptingEngine::println), asCALL_CDECL);
	assertNoAngelscriptError(r);
	
	// initialize default context
	auto handle = contextData_.create();
	auto& contextData = contextData_[handle];
	
	contextData.context = engine_->CreateContext();
}

asIScriptContext* ScriptingEngine::getContext(const ExecutionContextHandle& executionContextHandle) const
{
	if (executionContextHandle.index() >= contextData_.size())
	{
		throw Exception("ExecutionContextHandle is not valid");
	}
	
	auto context = contextData_[executionContextHandle].context;
	
	if (context == nullptr)
	{
		throw Exception("ExecutionContextHandle is not valid");
	}
	
	return context;
}

asIScriptModule* ScriptingEngine::createModuleFromScript(const std::string& moduleName, const std::string& scriptData) const
{
	CScriptBuilder builder = CScriptBuilder();
	builder.StartNewModule(engine_, moduleName.c_str());
	builder.AddSectionFromMemory("", scriptData.c_str(), scriptData.length());
	int32 r = builder.BuildModule();
	assertNoAngelscriptError(r);
	return builder.GetModule();
}

void ScriptingEngine::destroyModule(const std::string& moduleName)
{
	int32 r = engine_->DiscardModule(moduleName.c_str());
	assertNoAngelscriptError(r);
}

void ScriptingEngine::setArguments(asIScriptContext* context, ParameterList& arguments) const
{
	int32 r = 0;
	
	for ( size_t i = 0; i < arguments.size(); i++)
	{
		switch (arguments[i].type())
	    {
	        case ParameterType::TYPE_BOOL:
	            r = context->SetArgByte(i, arguments[i].value<bool>());
	            break;
	
	        case ParameterType::TYPE_INT8:
	            r = context->SetArgByte(i, arguments[i].value<int8>());
	            break;
	            
			case ParameterType::TYPE_UINT8:
			    r = context->SetArgByte(i, arguments[i].value<uint8>());
	            break;
	            
	        case ParameterType::TYPE_INT16:
				r = context->SetArgWord(i, arguments[i].value<int16>());
	            break;
	            
	        case ParameterType::TYPE_UINT16:
	            r = context->SetArgWord(i, arguments[i].value<uint16>());
	            break;
	
	        case ParameterType::TYPE_INT32:
				r = context->SetArgDWord(i, arguments[i].value<int32>());
	            break;
	            
	        case ParameterType::TYPE_UINT32:
	            r = context->SetArgDWord(i, arguments[i].value<uint32>());
	            break;
	
	        case ParameterType::TYPE_INT64:
				r = context->SetArgQWord(i, arguments[i].value<int64>());
	            break;
	            
	        case ParameterType::TYPE_UINT64:
	            r = context->SetArgQWord(i, arguments[i].value<uint64>());
	            break;
	
	        case ParameterType::TYPE_FLOAT32:
	            r = context->SetArgFloat(i, arguments[i].value<float32>());
	            break;
	           
	        case ParameterType::TYPE_FLOAT64:
	            r = context->SetArgDouble(i, arguments[i].value<float64>());
	            break;
	           
	        case ParameterType::TYPE_OBJECT_REF:
				r = context->SetArgAddress(i, arguments[i].pointer());
	            break;
	            
	        case ParameterType::TYPE_OBJECT_VAL:
				r = context->SetArgObject(i, arguments[i].pointer());
	            break;
			
	        default:
				throw Exception("Unknown parameter type.");
				break;
		}
		
		assertNoAngelscriptError(r);
	}
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, asIScriptFunction* function, asIScriptObject* object) const
{
	assert(function->GetParamCount() == 0);
	
	int32 r = context->Prepare(function);
	assertNoAngelscriptError(r);
	
	if (object != nullptr)
	{
		context->SetObject(object);
	}
	
	r = context->Execute();
	
	if ( r != asEXECUTION_FINISHED )
	{
		std::string msg = std::string();
		
		// The execution didn't complete as expected. Determine what happened.
		if ( r == asEXECUTION_EXCEPTION )
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			msg = std::string("An exception occurred: ");
			msg += GetExceptionInfo(context, true);
			//msg += std::string(context->GetExceptionString());
			throw Exception("ScriptEngine: " + msg);
		}
		
		assertNoAngelscriptError(r);
	}
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, asIScriptFunction* function, asIScriptObject* object, ParameterList& arguments) const
{	
	assert(function->GetParamCount() == arguments.size());
	
	int32 r = context->Prepare(function);
	assertNoAngelscriptError(r);
	
	if (arguments.size() != 0)
	{
		setArguments(context, arguments);
	}
	
	if (object != nullptr)
	{
		context->SetObject(object);
	}
	
	r = context->Execute();
	
	if ( r != asEXECUTION_FINISHED )
	{
		std::string msg = std::string();
		
		// The execution didn't complete as expected. Determine what happened.
		if ( r == asEXECUTION_EXCEPTION )
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			msg = std::string("An exception occurred: ");
			msg += GetExceptionInfo(context, true);
			//msg += std::string(context->GetExceptionString());
			throw Exception("ScriptEngine: " + msg);
		}
		
		assertNoAngelscriptError(r);
	}
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, asIScriptFunction* function) const
{
	callFunction(context, module, function, nullptr);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, asIScriptFunction* function, ParameterList& arguments) const
{
	callFunction(context, module, function, nullptr, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function) const
{
	auto func = getFunctionByDecl(function, module);
	
	callFunction(context, module, func);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function, ParameterList& arguments) const
{
	auto func = getFunctionByDecl(function, module);
	
	callFunction(context, module, func, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, const ScriptFunctionHandle& scriptFunctionHandle) const
{
	auto function = scriptFunctions_[scriptFunctionHandle.index()];
	
	callFunction(context, module, function);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments) const
{
	auto function = scriptFunctions_[scriptFunctionHandle.index()];
	
	callFunction(context, module, function, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptHandle& scriptHandle, const std::string& function) const
{
	auto module = moduleData_[scriptHandle].module;
	callFunction(context, module, function);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments) const
{
	auto module = moduleData_[scriptHandle].module;
	callFunction(context, module, function, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptHandle& scriptHandle, const ScriptFunctionHandle& scriptFunctionHandle) const
{
	auto module = moduleData_[scriptHandle].module;
	auto function = scriptFunctions_[scriptFunctionHandle.index()];
	
	callFunction(context, module, function);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptHandle& scriptHandle, const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments) const
{
	auto module = moduleData_[scriptHandle].module;
	auto function = scriptFunctions_[scriptFunctionHandle.index()];
	
	callFunction(context, module, function, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function) const
{
	auto module = getModule(scriptObjectHandle);
	auto object = scriptObjectData_[scriptObjectHandle].object;
	auto objectFunction = getMethod(scriptObjectHandle, function);
	
	callFunction(context, module, objectFunction, object);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments) const
{
	auto module = getModule(scriptObjectHandle);
	auto object = scriptObjectData_[scriptObjectHandle].object;
	auto objectFunction = getMethod(scriptObjectHandle, function);
	
	callFunction(context, module, objectFunction, object, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, ScriptObjectFunctionHandle& scriptObjectFunctionHandle) const
{
	auto module = getModule(scriptObjectHandle);
	auto object = scriptObjectData_[scriptObjectHandle].object;
	auto objectFunction = getMethod(scriptObjectFunctionHandle);
	
	callFunction(context, module, objectFunction, object);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments) const
{
	auto module = getModule(scriptObjectHandle);
	auto object = scriptObjectData_[scriptObjectHandle].object;
	auto objectFunction = getMethod(scriptObjectFunctionHandle);
	
	callFunction(context, module, objectFunction, object, arguments);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnObjectParser, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}

void ScriptingEngine::run(const std::string& filename, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto scriptData = fileSystem_->readAll(filename);
	execute(scriptData, function, arguments, returnValue, executionContextHandle);
}



/* EXECUTE SCRIPT DATA FUNCTIONS */
void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnObjectParser(context->GetReturnObject());
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnFloat();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnDouble();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnByte();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnByte();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnDWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnDWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnQWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnQWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnObjectParser(context->GetReturnObject());
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnFloat();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnDouble();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnByte();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnByte();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnDWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnDWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnQWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnQWord();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}



/* EXECUTE SCRIPT HANDLE FUNCTIONS */
void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptHandle& scriptHandle, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptHandle, function, arguments);
	returnValue = context->GetReturnQWord();
}



/* EXECUTE SCRIPT OBJECT HANDLE FUNCTIONS */
void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
	returnValue = context->GetReturnQWord();
}

ExecutionContextHandle ScriptingEngine::createExecutionContext()
{
	if (contextData_.size() == ScriptingEngine::MAX_EXECUTION_CONTEXTS)
	{
		throw Exception("Maximum number of execution contexts reached.");
	}
	
	auto handle = contextData_.create();
	auto& contextData = contextData_[handle];
	
	contextData.context = engine_->CreateContext();
	
	return handle;
}

ScriptObjectHandle ScriptingEngine::registerScriptObject(const ScriptHandle& scriptHandle, asIScriptObject* object)
{
	auto handle = scriptObjectData_.create();
	auto& scriptObjectData = scriptObjectData_[handle];
	
	scriptObjectData.scriptHandle = scriptHandle;
	scriptObjectData.className = object->GetObjectType()->GetName();
	scriptObjectData.object = object;
	
	return handle;
}

void ScriptingEngine::unregisterScriptObject(const ScriptObjectHandle& scriptObjectHandle)
{
	
}

void ScriptingEngine::unregisterAllScriptObjects()
{
	
}

void ScriptingEngine::registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall)
{
	int32 r = engine_->RegisterGlobalFunction(name.c_str(), funcPointer, callConv, objForThiscall);
	assertNoAngelscriptError(r);
}

void ScriptingEngine::registerGlobalProperty(const std::string& declaration, void* pointer)
{
	int32 r = engine_->RegisterGlobalProperty(declaration.c_str(), pointer);
	assertNoAngelscriptError(r);
}

void ScriptingEngine::registerClass(const std::string& name)
{
	registerObjectType(name.c_str(), 0, asOBJ_REF);
}

void ScriptingEngine::registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
	const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer)
{
	registerClass(name);
	registerClassFactory(name, classFactorySignature, classFactoryFuncPointer);
	registerClassAddRef(name, addRefSignature, addRefFuncPointer);
	registerClassReleaseRef(name, releaseRefSignature, releaseRefFuncPointer);
}
		
void ScriptingEngine::registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer)
{
	registerObjectBehaviour(name.c_str(), asBEHAVE_FACTORY, classFactorySignature.c_str(), classFactoryFuncPointer, asCALL_CDECL);
}

void ScriptingEngine::registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer)
{
	registerObjectBehaviour(name.c_str(), asBEHAVE_ADDREF, addRefSignature.c_str(), addRefFuncPointer, asCALL_THISCALL);
}

void ScriptingEngine::registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer)
{
	registerObjectBehaviour(name.c_str(), asBEHAVE_RELEASE, releaseRefSignature.c_str(), releaseRefFuncPointer, asCALL_THISCALL);
}
		
void ScriptingEngine::registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer)
{
	registerObjectMethod(className.c_str(), methodSignature.c_str(), funcPointer, asCALL_THISCALL);
}

void ScriptingEngine::registerInterface(const std::string& name)
{
	int32 r = engine_->RegisterInterface(name.c_str());
	
	if (r < 0)
	{
		std::string msg = std::string();

		if ( name.length() > 80 )
		{
			msg = std::string("Unable to register the interface: (cannot display name; it is too long!)");
		}
		else
		{
			msg = std::string("Unable to register the interface: ");
			msg += name;
		}
		
		throw Exception("ScriptEngine: " + msg);
	}
}

void ScriptingEngine::registerInterfaceMethod(const std::string& name, const std::string& declaration)
{
	int32 r = engine_->RegisterInterfaceMethod(name.c_str(), declaration.c_str());
	
	if (r < 0)
	{
		std::string msg = std::string();

		if ( name.length() > 80 )
		{
			msg = std::string("Unable to register the interface method: (cannot display declaration; it is too long!)");
		}
		else
		{
			msg = std::string("Unable to register the interface method: ");
			msg += declaration;
		}
		
		throw Exception("ScriptEngine: " + msg);
	}
}

void ScriptingEngine::registerEnum(const std::string& type)
{
	int32 r = engine_->RegisterEnum(type.c_str());
	
	if (r < 0)
	{
		std::string msg = std::string();

		if ( type.length() > 80 )
		{
			msg = std::string("Unable to register the enum: (cannot display type; it is too long!)");
		}
		else
		{
			msg = std::string("Unable to register the enum: ") + type;
		}
		
		throw Exception("ScriptEngine: " + msg);
	}
}

void ScriptingEngine::registerEnumValue(const std::string& type, const std::string& name, const int32 value)
{
	int32 r = engine_->RegisterEnumValue(type.c_str(), name.c_str(), value);
	
	if (r < 0)
	{
		std::string msg = std::string();

		if ( type.length() > 80 )
		{
			msg = std::string("Unable to register the enum value: (cannot display type; it is too long!)");
		}
		else
		{
			msg = std::string("Unable to register the enum value: ") + type;
		}
		
		throw Exception("ScriptEngine: " + msg);
	}
}

ScriptHandle ScriptingEngine::loadScript(const std::string& filename, const std::string& name)
{
	CScriptBuilder builder = CScriptBuilder();
	builder.StartNewModule(engine_, name.c_str());
	
	// error check
	if ( filename.length() > 150 )
	{
		throw Exception("ScriptEngine: Script filename too long.");
	}
	
	if (!fileSystem_->exists(filename))
	{
		throw std::runtime_error("Script file '" + filename + "' does not exist.");
	}

	// TODO: maybe make this a constant or something..?  Or make the directory an instance variable?
	//std::string file = fs::current_path().string() + std::string("/scripts/") + script;
	std::string file = std::string("../data/scripts/") + filename;
	
	int32 r = builder.AddSectionFromFile(file.c_str());
	assertNoAngelscriptError(r);
	
	r = builder.BuildModule();
	assertNoAngelscriptError(r);
	
	auto handle = moduleData_.create();
	auto& moduleData = moduleData_[handle];
	
	moduleData.module = builder.GetModule();
	
	return handle;
}

void ScriptingEngine::destroyScript(const std::string& name)
{
	
}

void ScriptingEngine::destroyScript(const ScriptHandle& scriptHandle)
{
	
}

void ScriptingEngine::destroyAllScripts()
{
	
}

void ScriptingEngine::registerObjectType(const std::string& obj, int32 byteSize, asDWORD flags)
{
	int32 r = engine_->RegisterObjectType(obj.c_str(), byteSize, flags);
	
	if (r < 0)
	{
		std::string msg = std::string();

		if ( obj.length() > 80 )
		{
			msg = std::string("Unable to register the object type: (cannot display 'obj' name; it is too long!)");
		}
		else
		{
			msg = std::string("Unable to register the object type: ");
			msg += obj;
		}
		
		throw Exception("ScriptEngine: " + msg);
	}
}

void ScriptingEngine::registerObjectProperty(const std::string& obj, const std::string& declaration, int32 byteOffset)
{
	int32 r = engine_->RegisterObjectProperty(obj.c_str(), declaration.c_str(), byteOffset);
	
	if (r < 0)
	{
		std::string msg = std::string();

		if ( obj.length() > 80 )
		{
			msg = std::string("Unable to register the object property: (cannot display 'obj' name; it is too long!)");
		}
		else
		{
			msg = std::string("Unable to register the object property: ");
			msg += obj;
		}
		
		throw Exception("ScriptEngine: " + msg);
	}
}

void ScriptingEngine::registerObjectMethod(const std::string& obj, const std::string& declaration,
									  const asSFuncPtr& funcPointer, asDWORD callConv)
{
	int32 r = engine_->RegisterObjectMethod(obj.c_str(), declaration.c_str(), funcPointer, callConv);
	
	if (r < 0)
	{
		std::string msg = std::string();

		if ( obj.length() > 80 )
		{
			msg = std::string("Unable to register the object method: (cannot display 'obj' name; it is too long!)");
		}
		else
		{
			msg = std::string("Unable to register the object method: ");
			msg += obj;
		}
		
		throw Exception("ScriptEngine: " + msg);
	}
}

void ScriptingEngine::registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour,
										 const std::string& declaration, const asSFuncPtr& funcPointer, asDWORD callConv)
{
	int32 r = engine_->RegisterObjectBehaviour(obj.c_str(), behaviour, declaration.c_str(), funcPointer, callConv);

	if ( r < 0 )
	{
		std::string msg = std::string();

		if ( obj.length() > 40 || declaration.length() > 40 )
		{
			msg = std::string("Unable to register the object behaviour: (cannot display 'obj' and 'declaration' names; they are too long!)");
		}
		else
		{
			msg = std::string("Unable to register the object (");
			msg += obj;
			msg += std::string(") behaviour: ");
			msg += declaration;
		}

		throw Exception("ScriptEngine: " + msg);
	}
}

void ScriptingEngine::destroy()
{
	for ( auto c : contextData_ )
	{
		if (c.context != nullptr)
		{
			c.context->Release();
		}
	}
	
	unregisterAllScriptObjects();
	destroyAllScripts();
	
	engine_->ShutDownAndRelease();
}

asIScriptFunction* ScriptingEngine::getFunctionByDecl(const std::string& function, asIScriptModule* module) const
{
	auto func = module->GetFunctionByDecl( function.c_str() );
	
	if ( func == nullptr )
	{
		// The function couldn't be found. Instruct the script writer to include the expected function in the script.
		std::string msg = std::string();

		if ( function.length() > 80 )
		{
			msg = std::string("Function name too long.");
		}
		else
		{
			msg = std::string("Unable to locate the specified function: ");
			msg += function;
		}

		throw Exception("ScriptEngine: " + msg);
	}
	
	return func;
}

asIScriptModule* ScriptingEngine::getModule(const ScriptObjectHandle& scriptObjectHandle) const
{
	const auto scriptObjectData = scriptObjectData_[scriptObjectHandle];
	
	auto module = moduleData_[scriptObjectData.scriptHandle].module;
	
	return module;
}

asITypeInfo* ScriptingEngine::getType(const ScriptObjectHandle& scriptObjectHandle) const
{
	const auto& scriptObjectData = scriptObjectData_[scriptObjectHandle];
	
	auto module = moduleData_[scriptObjectData.scriptHandle].module;
	
	auto type = module->GetTypeInfoByDecl(scriptObjectData.className.c_str());
	
	return type;
}

asIScriptFunction* ScriptingEngine::getMethod(const ScriptObjectHandle& scriptObjectHandle, const std::string& function) const
{
	auto module = getModule(scriptObjectHandle);
	auto type = getType(scriptObjectHandle);
	
	return type->GetMethodByDecl(function.c_str());
}

asIScriptFunction* ScriptingEngine::getMethod(const ScriptObjectFunctionHandle& scriptObjectFunctionHandle) const
{
	auto objectFunction = scriptObjectFunctions_[scriptObjectFunctionHandle.index()];
	
	return objectFunction;
}

// Implement a simple message callback function
void ScriptingEngine::MessageCallback(const asSMessageInfo* msg, void* param)
{
	std::string type = std::string("ERR ");

	if ( msg->type == asMSGTYPE_WARNING )
	{
		type = std::string("WARN");
	}
	else if ( msg->type == asMSGTYPE_INFORMATION )
	{
		type = std::string("INFO");
	}
	
	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type.c_str(), msg->message);
}

// Print the script string to the standard output stream
void ScriptingEngine::print(const std::string& msg)
{
	printf("%s", msg.c_str());
}

void ScriptingEngine::println(const std::string& msg)
{
	printf("%s\n", msg.c_str());
}

void ScriptingEngine::discardModule(const std::string& name)
{
	int32 r = engine_->DiscardModule( name.c_str() );
	assertNoAngelscriptError(r);
}

}
}
}
