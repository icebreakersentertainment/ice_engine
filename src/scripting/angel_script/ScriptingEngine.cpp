#include <iostream>
#include <stdio.h>

#include <boost/format.hpp>

#include "scripting/angel_script/ScriptingEngine.hpp"

#include "scripting/angel_script/scripthelper/scripthelper.h"
#include "scripting/angel_script/scriptarray/scriptarray.h"
#include "scripting/angel_script/scriptstdstring/scriptstdstring.h"
//#include "scripting/angel_script/glm_bindings/Vec3.h"
#include "scripting/angel_script/scriptdictionary/scriptdictionary.h"
#include "scripting/angel_script/scripthandle/scripthandle.h"
#include "scripting/angel_script/scriptvector/scriptvector.hpp"
#include "scripting/angel_script/scriptglm/scriptglm.hpp"
#include "scripting/angel_script/scriptchrono/scriptchrono.hpp"

#include "exceptions/Exception.hpp"
#include "exceptions/InvalidArgumentException.hpp"

#include "Platform.hpp"

namespace ice_engine
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

	RegisterScriptDictionary(engine_);

	RegisterScriptHandle(engine_);

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
	RegisterVectorBindings<std::string>(engine_, "vectorString", "string");

	RegisterGlmBindings(engine_);

	RegisterChronoBindings(engine_);

	RegisterVectorBindings<glm::vec2>(engine_, "vectorVec2", "vec2");
	RegisterVectorBindings<glm::vec3>(engine_, "vectorVec3", "vec3");
	RegisterVectorBindings<glm::vec4>(engine_, "vectorVec4", "vec4");
	RegisterVectorBindings<glm::ivec2>(engine_, "vectorIVec2", "ivec2");
	RegisterVectorBindings<glm::ivec3>(engine_, "vectorIVec3", "ivec3");
	RegisterVectorBindings<glm::ivec4>(engine_, "vectorIVec4", "ivec4");
	RegisterVectorBindings<glm::quat>(engine_, "vectorMat4", "mat4");
	RegisterVectorBindings<glm::quat>(engine_, "vectorQuat", "quat");

	r = engine_->RegisterInterface("IScriptObject");
	assertNoAngelscriptError(r);

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
	if (executionContextHandle.id() != 0 && !contextData_.valid(executionContextHandle))
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

asIScriptModule* ScriptingEngine::createModuleFromScript(const std::string& moduleName, const std::string& scriptData)
{
	std::vector<std::string> scriptDataVector;
	scriptDataVector.push_back(scriptData);

	return createModuleFromScripts(moduleName, scriptDataVector);
}

asIScriptModule* ScriptingEngine::createModuleFromScripts(const std::string& moduleName, const std::vector<std::string>& scriptData)
{
	CScriptBuilder builder = CScriptBuilder();

#if defined(PLATFORM_WINDOWS)
	builder.DefineWord("PLATFORM_WINDOWS");
#elif defined(PLATFORM_MAC)
	builder.DefineWord("PLATFORM_MAC");
	#define PLATFORM_MAC
#elif defined(PLATFORM_LINUX)
	builder.DefineWord("PLATFORM_LINUX");
#endif

	builder.StartNewModule(engine_, moduleName.c_str());

	for (const auto& data : scriptData)
	{
		builder.AddSectionFromMemory("", data.c_str(), data.length());
	}

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

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptFunction* function, asIScriptObject* object) const
{
	assert(function->GetParamCount() == 0);

	if (context->GetState() == asEContextState::asEXECUTION_ACTIVE)
	{
		int32 r = context->PushState();
		assertNoAngelscriptError(r);
	}

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

	if (context->IsNested())
	{
		int32 r = context->PopState();
		assertNoAngelscriptError(r);
	}
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptFunction* function, asIScriptObject* object, ParameterList& arguments) const
{
	assert(function->GetParamCount() == arguments.size());

	if (context->GetState() == asEContextState::asEXECUTION_ACTIVE)
	{
		int32 r = context->PushState();
		assertNoAngelscriptError(r);
	}

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

	if (context->IsNested())
	{
		int32 r = context->PopState();
		assertNoAngelscriptError(r);
	}
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptFunction* function) const
{
	callFunction(context, function, nullptr);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptFunction* function, ParameterList& arguments) const
{
	callFunction(context, function, nullptr, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function) const
{
	auto func = getFunctionByDecl(function, module);

	callFunction(context, func);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function, ParameterList& arguments) const
{
	auto func = getFunctionByDecl(function, module);

	callFunction(context, func, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptFunctionHandle& scriptFunctionHandle) const
{
	auto function = static_cast<asIScriptFunction*>(scriptFunctionHandle.get());

	callFunction(context, function);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments) const
{
	auto function = static_cast<asIScriptFunction*>(scriptFunctionHandle.get());

	callFunction(context, function, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ModuleHandle& moduleHandle, const std::string& function) const
{
	auto module = moduleData_[moduleHandle].module;
	callFunction(context, module, function);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments) const
{
	auto module = moduleData_[moduleHandle].module;
	callFunction(context, module, function, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function) const
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());
	auto objectFunction = getMethod(scriptObjectHandle, function);

	callFunction(context, objectFunction, object);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments) const
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());

	auto objectFunction = getMethod(scriptObjectHandle, function);

	callFunction(context, objectFunction, object, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle) const
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());
	auto objectFunction = static_cast<asIScriptFunction*>(scriptObjectFunctionHandle.get());

	callFunction(context, objectFunction, object);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments) const
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());
	auto objectFunction = static_cast<asIScriptFunction*>(scriptObjectFunctionHandle.get());

	callFunction(context, objectFunction, object, arguments);
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
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnObjectParser(context->GetReturnObject());
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnFloat();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnDouble();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnByte();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnByte();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnDWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnDWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnQWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function);
	returnValue = context->GetReturnQWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnObjectParser(context->GetReturnObject());
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnFloat();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnDouble();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnByte();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnByte();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnDWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnDWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnQWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}

void ScriptingEngine::execute(const std::string& scriptData, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	auto module = createModuleFromScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, scriptData);
	callFunction(context, module, function, arguments);
	returnValue = context->GetReturnQWord();
	context->Unprepare();
	destroyModule(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME);
}



/* EXECUTE SCRIPT FUNCTION HANDLE FUNCTIONS */
void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptFunctionHandle, arguments);
	returnValue = context->GetReturnQWord();
}



/* EXECUTE SCRIPT HANDLE FUNCTIONS */
void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, moduleHandle, function, arguments);
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

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, function, arguments);
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



/* EXECUTE SCRIPT OBJECT FUNCTION HANDLE FUNCTIONS */
void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, std::function<void(void*)> returnObjectParser, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnObjectParser(context->GetReturnObject());
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, float32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnValue = context->GetReturnFloat();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, float64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnValue = context->GetReturnDouble();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint8& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnValue = context->GetReturnByte();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint16& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnValue = context->GetReturnWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint32& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnValue = context->GetReturnDWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, int64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
	returnValue = context->GetReturnQWord();
}

void ScriptingEngine::execute(const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments, uint64& returnValue, const ExecutionContextHandle& executionContextHandle)
{
	auto context = getContext(executionContextHandle);
	callFunction(context, scriptObjectHandle, scriptObjectFunctionHandle, arguments);
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

void ScriptingEngine::destroyExecutionContext(const ExecutionContextHandle& executionContextHandle)
{
	contextData_.destroy(executionContextHandle);
}

ModuleHandle ScriptingEngine::createModule(const std::string& name, const std::vector<std::string>& scriptData)
{
	auto handle = moduleData_.create();
	auto& moduleData = moduleData_[handle];

	moduleData.module = createModuleFromScripts(name, scriptData);

	return handle;
}

std::string ScriptingEngine::getScriptObjectName(const ScriptObjectHandle& scriptObjectHandle) const
{
	auto type = getType(scriptObjectHandle);

	return type->GetName();
}

ScriptObjectHandle ScriptingEngine::createUninitializedScriptObject(const ModuleHandle& moduleHandle, const std::string& name)
{
	auto& moduleData = moduleData_[moduleHandle];

	auto type = moduleData.module->GetTypeInfoByName(name.c_str());

	if (!type)
	{
		throw std::runtime_error("Type with name " + name + " not found.");
	}

	return ScriptObjectHandle(engine_->CreateUninitializedScriptObject(type));
}

void ScriptingEngine::destroyModule(const ModuleHandle& moduleHandle)
{
	auto& moduleData = moduleData_[moduleHandle];

	LOG_TRACE(logger_, "Releasing module: %s", moduleData.module->GetName());

	moduleData.module->Discard();

	moduleData_.destroy(moduleHandle);
}

void ScriptingEngine::releaseScriptObject(const ScriptObjectHandle& scriptObjectHandle)
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());

	LOG_TRACE(logger_, "Releasing script object: %s", object->GetObjectType()->GetName());

	object->Release();
}

void ScriptingEngine::releaseAllScriptObjects()
{
	LOG_TRACE(logger_, "Releasing all script objects");
}

void ScriptingEngine::releaseScriptObjectFunction(const ScriptObjectFunctionHandle& scriptObjectFunctionHandle)
{
	auto object = static_cast<asIScriptFunction*>(scriptObjectFunctionHandle.get());

	LOG_TRACE(logger_, "Releasing script object function: %s", object->GetObjectType()->GetName());

	object->Release();
}

void ScriptingEngine::releaseAllScriptObjectFunctions()
{
	LOG_TRACE(logger_, "Releasing all script object functions");
}

void ScriptingEngine::releaseScriptFunction(const ScriptFunctionHandle& scriptFunctionHandle)
{
	auto function = static_cast<asIScriptFunction*>(scriptFunctionHandle.get());

	function->Release();
}

void ScriptingEngine::releaseAllScriptFunctions()
{
	LOG_TRACE(logger_, "Releasing all script functions");
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

void ScriptingEngine::registerFunctionDefinition(const std::string& name)
{
	int32 r = engine_->RegisterFuncdef(name.c_str());

	if (r < 0)
	{
		std::string msg = std::string();

		if ( name.length() > 80 )
		{
			msg = std::string("Unable to register function definition: (cannot display name; it is too long!)");
		}
		else
		{
			msg = std::string("Unable to register function definition: ");
			msg += name;
		}

		throw Exception("ScriptEngine: " + msg);
	}
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

void ScriptingEngine::registerEnumValue(const std::string& type, const std::string& name, const int64 value)
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

void ScriptingEngine::destroyAllModules()
{
	LOG_TRACE(logger_, "Destroying all modules");
	for ( auto& m : moduleData_ )
	{
		LOG_TRACE(logger_, std::string("Destroying module with name ") + m.module->GetName());
		m.module->Discard();
	}

	moduleData_.clear();
}

ScriptFunctionHandle ScriptingEngine::getScriptFunction(const ModuleHandle& moduleHandle, const std::string& function)
{
	auto& moduleData = moduleData_[moduleHandle];

	auto scriptFunctionObject = getFunctionByDecl(function, moduleData.module);

	return ScriptFunctionHandle(scriptFunctionObject);
}

ScriptObjectFunctionHandle ScriptingEngine::getScriptObjectFunction(const ScriptObjectHandle& scriptObjectHandle, const std::string& function)
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());

	auto scriptFunctionObject = getFunctionByDecl(function, object);

	scriptFunctionObject->AddRef();

	return ScriptObjectFunctionHandle(scriptFunctionObject);
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
									  const asSFuncPtr& funcPointer, asDWORD callConv, void* auxiliary)
{
	int32 r = engine_->RegisterObjectMethod(obj.c_str(), declaration.c_str(), funcPointer, callConv, auxiliary);

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
	LOG_DEBUG(logger_, "Destroying scripting engine");

	LOG_TRACE(logger_, "Destroying contexts");
	for ( auto c : contextData_ )
	{
		if (c.context != nullptr)
		{
			c.context->Release();
		}
	}

	releaseAllScriptObjects();
	destroyAllModules();

	LOG_TRACE(logger_, "Shutting down and releasing Angelscript engine");
	engine_->ShutDownAndRelease();
}

asIScriptFunction* ScriptingEngine::getFunctionByDecl(const std::string& function, const asIScriptModule* module) const
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

asIScriptFunction* ScriptingEngine::getFunctionByDecl(const std::string& function, const asIScriptObject* object) const
{
	auto func = object->GetObjectType()->GetMethodByDecl(function.c_str());

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
	/*
	const auto scriptObjectData = scriptObjectData_[scriptObjectHandle];

	auto module = moduleData_[scriptObjectData.moduleHandle].module;

	return module;
	*/

	return nullptr;
}

asITypeInfo* ScriptingEngine::getType(const ScriptObjectHandle& scriptObjectHandle) const
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());

	return object->GetObjectType();
}

asIScriptFunction* ScriptingEngine::getMethod(const ScriptObjectHandle& scriptObjectHandle, const std::string& function) const
{
	//auto module = getModule(scriptObjectHandle);
	auto type = getType(scriptObjectHandle);

	return type->GetMethodByDecl(function.c_str());
}

// Implement a simple message callback function
void ScriptingEngine::MessageCallback(const asSMessageInfo* msg, void* param)
{
	std::string type = std::string("ERR ");

	if ( msg->type == asMSGTYPE_WARNING )
	{
		type = std::string("WARN");
		LOG_WARN(logger_, boost::format("%s (%d, %d) : %s") % msg->section %  msg->row %  msg->col %  msg->message );
	}
	else if ( msg->type == asMSGTYPE_INFORMATION )
	{
		type = std::string("INFO");
		LOG_INFO(logger_, boost::format("%s (%d, %d) : %s") % msg->section %  msg->row %  msg->col %  msg->message );
	}
	else
	{
		LOG_ERROR(logger_, boost::format("%s (%d, %d) : %s") % msg->section %  msg->row %  msg->col %  msg->message );
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
