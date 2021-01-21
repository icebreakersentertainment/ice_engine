#include <iostream>
#include <stdio.h>

#include <glm/gtx/string_cast.hpp>

#include <boost/exception/diagnostic_information.hpp>

#include "scripting/angel_script/ScriptingEngine.hpp"

#include "scripting/angel_script/scripthelper/scripthelper.h"
#include "scripting/angel_script/scriptarray/scriptarray.h"
#include "scripting/angel_script/scriptstdstring/scriptstdstring.h"
//#include "scripting/angel_script/glm_bindings/Vec3.h"
#include "scripting/angel_script/scriptdictionary/scriptdictionary.h"
//#include "scripting/angel_script/scripthandle/scripthandle.h"
#include "scripting/angel_script/scriptvector/scriptvector.hpp"
#include "scripting/angel_script/scriptglm/scriptglm.hpp"
#include "scripting/angel_script/scriptchrono/scriptchrono.hpp"

#include "scripting/angel_script/AngelscriptCPreProcessor.hpp"

#include "Platform.hpp"

namespace ice_engine
{
namespace scripting
{
namespace angel_script
{

namespace
{

void translateException(asIScriptContext *ctx, void* /*userParam*/)
{
    try
    {
        // Retrow the original exception so we can catch it again
        throw;
    }
    catch (const std::exception& e)
    {
        // Tell the VM the type of exception that occurred
        const auto diagnosticInformation = boost::diagnostic_information(e);
        ctx->SetException(diagnosticInformation.c_str());
    }
    catch (...)
    {
        // The callback must not allow any exception to be thrown, but it is not necessary
        // to explicitly set an exception string if the default exception string is sufficient
    }
}

/*
class IceEngineScriptBuilder : public CScriptBuilder
{
public:
    IceEngineScriptBuilder(fs::IFileSystem* fileSystem) : fileSystem_(fileSystem)
    {

    }

protected:
    // Returns 1 if the section was included
    // Returns 0 if the section was not included because it had already been included before
    // Returns <0 if there was an error
    int AddSectionFromFile(const char *filename) override
    {
        // The file name stored in the set should be the fully resolved name because
        // it is possible to name the same file in multiple ways using relative paths.
//        string fullpath = GetAbsolutePath(filename);
        const std::string fullpath = fileSystem_->getCanonicalPath(filename);

        if( IncludeIfNotAlreadyIncluded(fullpath.c_str()) )
        {
//            int r = LoadScriptSection(fullpath.c_str());
            int r = LoadScriptSection(filename);
            if( r < 0 )
                return r;
            else
                return 1;
        }

        return 0;
    }

    int LoadScriptSection(const char *filename) override
    {
        // Open the script file
//        string scriptFile = filename;
//#if _MSC_VER >= 1500 && !defined(__S3E__)
//        FILE *f = 0;
//        fopen_s(&f, scriptFile.c_str(), "rb");
//#else
//        FILE *f = fopen(scriptFile.c_str(), "rb");
//#endif

        try
        {
            const auto code = fileSystem_->readAll(filename);

            // Process the script section even if it is zero length so that the name is registered
            return ProcessScriptSection(code.c_str(), (unsigned int)(code.length()), filename, 0);
        }
        catch (const std::exception& e)
        {
            std::string msg = std::string("Failed to open script file '") + filename + "': " + e.what();
            engine->WriteMessage(filename, 0, 0, asMSGTYPE_ERROR, msg.c_str());

            // TODO: Write the file where this one was included from

            return -1;
        }


//        if( f == 0 )
//        {
//            // Write a message to the engine's message callback
//            string msg = "Failed to open script file '" + GetAbsolutePath(scriptFile) + "'";
//            engine->WriteMessage(filename, 0, 0, asMSGTYPE_ERROR, msg.c_str());
//
//            // TODO: Write the file where this one was included from
//
//            return -1;
//        }

        // Determine size of the file
//        fseek(f, 0, SEEK_END);
//        int len = ftell(f);
//        fseek(f, 0, SEEK_SET);

        // On Win32 it is possible to do the following instead
        // int len = _filelength(_fileno(f));

        // Read the entire file
//        string code;
//        size_t c = 0;
//        if( len > 0 )
//        {
//            code.resize(len);
//            c = fread(&code[0], len, 1, f);
//        }
//
//        fclose(f);
//
//        if( c == 0 && len > 0 )
//        {
//            // Write a message to the engine's message callback
//            string msg = "Failed to load script file '" + GetAbsolutePath(scriptFile) + "'";
//            engine->WriteMessage(filename, 0, 0, asMSGTYPE_ERROR, msg.c_str());
//            return -1;
//        }
    }

private:
    fs::IFileSystem* fileSystem_;
};
*/

template<typename T>
class HandleRegisterHelper
{
public:
    static void DefaultConstructor(T* memory) { new(memory) T(); }

    static void InitConstructor(T* memory, const uint64 id) { new(memory) T(id); }

    static void CopyConstructor(T* memory, const T& other) { new(memory) T(other); }
    static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }
};

/**
 * Register our Handle bindings.
 */
template<typename T>
void registerHandleBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name)
{
    typedef HandleRegisterHelper<T> HandleBase;

    scriptingEngine->registerObjectType(name.c_str(), sizeof(T), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<T>());
    scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(HandleBase::DefaultConstructor), asCALL_CDECL_OBJFIRST);
    scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(HandleBase::InitConstructor), asCALL_CDECL_OBJFIRST);
    scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f(const " + name + "& in)", asFUNCTION(HandleBase::CopyConstructor), asCALL_CDECL_OBJFIRST);
    scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(HandleBase::DefaultDestructor), asCALL_CDECL_OBJFIRST);
    scriptingEngine->registerClassMethod(name.c_str(), name + "& opAssign(const " + name + "& in)", asMETHODPR(T, operator=, (const T&), T&));
    scriptingEngine->registerClassMethod(name.c_str(), "uint64 id() const", asMETHODPR(T, id, () const, uint64));
    scriptingEngine->registerClassMethod(name.c_str(), "uint32 index() const", asMETHODPR(T, index, () const, uint32));
    scriptingEngine->registerClassMethod(name.c_str(), "uint32 version() const", asMETHODPR(T, version, () const, uint32));
    scriptingEngine->registerClassMethod(name.c_str(), "bool opImplConv() const", asMETHODPR(T, operator bool, () const, bool ));
    scriptingEngine->registerClassMethod(name.c_str(), "bool opEquals(const " + name + "& in) const", asMETHODPR(T, operator==, (const T&) const, bool));
}

template<typename T, typename K, typename V>
class UnorderedMapRegisterHelper
{
public:
    static void DefaultConstructor(T* memory) { new(memory) T(); }
    static void CopyConstructor(const T& other, T* memory) { new(memory) T(other); }
    static void DefaultDestructor(T* memory) { ((T*)memory)->~T(); }

    static T& assignmentOperator(const T& other, T* m) { (*m) = other; return *m; }

    static uint64 size(T* m) { return static_cast<uint64>(m->size()); }
    static V& at(const K& k, T* m) { return m->at(k); }
    static V& index(const K& k, T* m) { return (*m)[k]; }
    static void clear(T* m) { m->clear(); }
    static bool empty(T* m) { return m->empty(); }
    static uint64 max_size(T* m) { return static_cast<uint64>(m->max_size()); }
};

/**
 * Register our map bindings.
 */
template<typename K, typename V>
void registerUnorderedMapBindings(scripting::IScriptingEngine* scriptingEngine, const std::string& name, const std::string& keyType, const std::string& valueType, asEObjTypeFlags objectTypeFlags = asOBJ_APP_CLASS_ALLINTS)
{

    typedef UnorderedMapRegisterHelper<std::unordered_map<K, V>, K, V> UnorderedMapBase;

    scriptingEngine->registerObjectType(name.c_str(), sizeof(std::unordered_map<K, V>), asOBJ_VALUE | objectTypeFlags | asGetTypeTraits<std::unordered_map<K, V>>());

    scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(UnorderedMapBase::DefaultConstructor), asCALL_CDECL_OBJLAST);
    auto copyConstructorString = std::string("void f(const ") + name + "& in)";
    scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_CONSTRUCT, copyConstructorString.c_str(), asFUNCTION(UnorderedMapBase::CopyConstructor), asCALL_CDECL_OBJLAST);
    scriptingEngine->registerObjectBehaviour(name.c_str(), asBEHAVE_DESTRUCT, "void f()", asFUNCTION(UnorderedMapBase::DefaultDestructor), asCALL_CDECL_OBJLAST);

    auto assignmentOperatorFunctionString = name + std::string("& opAssign(const ") + name + "& in)";
    scriptingEngine->registerObjectMethod(name.c_str(), assignmentOperatorFunctionString.c_str(), asFUNCTION(UnorderedMapBase::assignmentOperator), asCALL_CDECL_OBJLAST);
    scriptingEngine->registerObjectMethod(name.c_str(), "uint64 size() const", asFUNCTION(UnorderedMapBase::size), asCALL_CDECL_OBJLAST);
    auto atFunctionString = valueType + "& at(" + keyType + ")";
    scriptingEngine->registerObjectMethod(name.c_str(), atFunctionString.c_str(), asFUNCTION(UnorderedMapBase::at), asCALL_CDECL_OBJLAST);
    auto atConstFunctionString = std::string("const ") + valueType + "& at(" + keyType + ") const";
    scriptingEngine->registerObjectMethod(name.c_str(), atConstFunctionString.c_str(), asFUNCTION(UnorderedMapBase::at), asCALL_CDECL_OBJLAST);
    auto indexFunctionString = valueType + "& opIndex(" + keyType + ")";
    scriptingEngine->registerObjectMethod(name.c_str(), indexFunctionString.c_str(), asFUNCTION(UnorderedMapBase::index), asCALL_CDECL_OBJLAST);
    auto indexConstFunctionString = std::string("const ") + valueType + "& opIndex(" + keyType + ") const";
    scriptingEngine->registerObjectMethod(name.c_str(), indexConstFunctionString.c_str(), asFUNCTION(UnorderedMapBase::index), asCALL_CDECL_OBJLAST);
    scriptingEngine->registerObjectMethod(name.c_str(), "void clear()", asFUNCTION(UnorderedMapBase::clear), asCALL_CDECL_OBJLAST);
    scriptingEngine->registerObjectMethod(name.c_str(), "bool empty() const", asFUNCTION(UnorderedMapBase::empty), asCALL_CDECL_OBJLAST);
    scriptingEngine->registerObjectMethod(name.c_str(), "uint64 max_size() const", asFUNCTION(UnorderedMapBase::max_size), asCALL_CDECL_OBJLAST);
}

template<typename T>
std::function<std::string(void*)> scriptingEngineDebuggerToStringCallback()
{
    return [](void* value) {
        std::stringstream ss;
        ss << *static_cast<T*>(value);
        return ss.str();
    };
}
}

const std::string ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME = std::string("ONE_TIME_RUN_SCRIPT_MODULE_NAME");

ScriptingEngine::ScriptingEngine(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger)
:
properties_(properties), fileSystem_(fileSystem), logger_(logger), debugger_(std::make_unique<AngelscriptDebugger>(logger_)), ctx_(nullptr)
{
	initialize();
}

ScriptingEngine::~ScriptingEngine()
{
	destroy();
}

void ScriptingEngine::initialize()
{
	engine_ = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	engine_->SetEngineProperty(asEP_AUTO_GARBAGE_COLLECT, false);

    engine_->SetTranslateAppExceptionCallback(asFUNCTION(translateException), 0, asCALL_CDECL);

	// Set the message callback to receive information on errors in human readable form.
	int32 r = engine_->SetMessageCallback(asMETHOD(ScriptingEngine, MessageCallback), this, asCALL_THISCALL);
	assertNoAngelscriptError(r);

	/* ScriptingEngine doesn't have a built-in string type, as there is no definite standard
	 * string type for C++ applications. Every developer is free to register it's own string type.
	 * The SDK do however provide a standard add-on for registering a string type, so it's not
	 * necessary to implement the registration yourself if you don't want to.
	 */
	RegisterStdString(engine_);

    RegisterExceptionRoutines(engine_);

    debugger_->registerToStringCallback("string", scriptingEngineDebuggerToStringCallback<std::string>());

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
    debugger_->registerToStringCallback("vec3", [](void* value) { return glm::to_string(*static_cast<glm::vec3*>(value)); });

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

	// TEST
    registerHandleBindings<ExecutionContextHandle>(this, "ExecutionContextHandle");
    registerHandleBindings<ModuleHandle>(this, "ModuleHandle");

    registerUnorderedMapBindings<std::string, std::string>(this, "unordered_mapStringString", "string", "string");

    registerGlobalFunction(
        "ModuleHandle createModule(const string& in, const vectorString& in, const unordered_mapStringString& in = unordered_mapStringString())",
        asMETHOD(ScriptingEngine, createModule),
        asCALL_THISCALL_ASGLOBAL,
        this
    );
    registerGlobalFunction(
        "ModuleHandle getModule(const string& in)",
        asMETHODPR(ScriptingEngine, getModule, (const std::string&) const, ModuleHandle),
        asCALL_THISCALL_ASGLOBAL,
        this
    );
    registerGlobalFunction(
        "void destroyModule(const ModuleHandle& in)",
        asMETHODPR(ScriptingEngine, destroyModule, (const ModuleHandle&), void),
        asCALL_THISCALL_ASGLOBAL,
        this
    );
    registerGlobalFunction(
        "ref@ createScriptObject(const ModuleHandle& in, const string& in, const string& in, const ExecutionContextHandle& in =  ExecutionContextHandle(0))",
        asMETHOD(ScriptingEngine, createScriptObjectReturnAsScriptHandle),
        asCALL_THISCALL_ASGLOBAL,
        this
    );

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
	return createModuleFromScripts(moduleName, {scriptData});
}

asIScriptModule* ScriptingEngine::createModuleFromScripts(const std::string& moduleName, const std::vector<std::string>& scriptData, const std::unordered_map<std::string, std::string>& includeOverrides)
{
    const auto existingModule = engine_->GetModule(moduleName.c_str());
    if (existingModule != nullptr)
    {
        throw InvalidArgumentException(std::string("Module with name '") + moduleName + "' already exists.");
    }

//		auto testData = fileSystem->readAll("bootstrap.as", ice_engine::fs::FileFlags::READ);

    AngelscriptCPreProcessor cpp{fileSystem_, logger_, includeOverrides};

    std::stringstream ss;
    for (const auto& data : scriptData)
    {
        ss << data << std::endl;
    }

    const auto source = ss.str();

//    std::cout << "BEFORE " << std::endl;
//    std::cout << source << std::endl;

    const std::unordered_map<std::string, std::string> defineMap = {
#if defined(PLATFORM_WINDOWS)
            {"PLATFORM_WINDOWS", "1"}
#elif defined(PLATFORM_MAC)
            {"PLATFORM_MAC", "1"}
#elif defined(PLATFORM_LINUX)
            {"PLATFORM_LINUX", "1"}
#endif
    };
    const auto result = cpp.process(source, defineMap, true, true);
    const auto processedFileSources = cpp.getProcessedFileSources();
//
////    std::cout << "AFTER" << std::endl;
////    std::cout << result << std::endl;
    {
//        auto ofs = std::ofstream("source." + moduleName + ".tmp");
//        ofs << result;
//        ofs.close();
    }

//    exit(0);

	CScriptBuilder builder = CScriptBuilder();
//    IceEngineScriptBuilder builder = IceEngineScriptBuilder(fileSystem_);

//#if defined(PLATFORM_WINDOWS)
//	builder.DefineWord("PLATFORM_WINDOWS");
//#elif defined(PLATFORM_MAC)
//	builder.DefineWord("PLATFORM_MAC");
//	#define PLATFORM_MAC
//#elif defined(PLATFORM_LINUX)
//	builder.DefineWord("PLATFORM_LINUX");
//#endif

	builder.StartNewModule(engine_, moduleName.c_str());

//	for (const auto& data : scriptData)
//	{
//		builder.AddSectionFromMemory("", data.c_str(), data.length());
//	}

    int a = 0;
//    auto ofs2 = std::ofstream("source.ALL.tmp");
	for (const auto& processedFileSource : processedFileSources)
	{
	    ++a;
//	    if (a > 100 || a == 40) continue;
//        std::cout << "AddSectionFromMemory (" << a << " / " << processedFileSources.size() << ") " << processedFileSource.second.size() << ": " << processedFileSource.first << std::endl;

//        auto n = boost::filesystem::path(processedFileSource.first).filename().string();
//        auto ofs = std::ofstream("source." + n + ".tmp");
////        ofs << "// " << processedFileSource.first << std::endl;
//        ofs2 << "//" << processedFileSource.first << std::endl;
//        ofs << processedFileSource.second << std::endl;
//        ofs2 << processedFileSource.second << std::endl;
//        ofs.close();

        if (processedFileSource.second.empty())
        {
//            std::cout << "processedFileSource.second" << std::endl;
            continue;
        }

//        if (processedFileSource.first == testFilename)
//        {
//            std::cout << "processedFileSource.first: " << processedFileSource.first << std::endl;
//        }

        const int32 r = builder.AddSectionFromMemory(processedFileSource.first.c_str(), processedFileSource.second.c_str(), static_cast<uint32>(processedFileSource.second.length()));
//        const int32 r = builder.AddSectionFromMemory("", processedFileSource.second.c_str(), processedFileSource.second.length());
//        std::cout << "AddSectionFromMemory done " << r << std::endl;
        assertNoAngelscriptError(r);
	}

//    builder.AddSectionFromMemory(processedFileSources[0].first.c_str(), processedFileSources[0].second.c_str(), processedFileSource[0].second.length());

//    if (!result.empty())
//    {
//        builder.AddSectionFromMemory("", result.c_str(), result.length());
//    }

//    std::cout << "BuildModule" << std::endl;
	int32 r = builder.BuildModule();
//    std::cout << "BuildModule done " << r << std::endl;
	assertNoAngelscriptError(r);
	return builder.GetModule();
}

void ScriptingEngine::destroyModule(const std::string& moduleName)
{
	int32 r = engine_->DiscardModule(moduleName.c_str());
	assertNoAngelscriptError(r);
}

CScriptHandle ScriptingEngine::createScriptObjectReturnAsScriptHandle(const ModuleHandle& moduleHandle, const std::string& objectName, const std::string& factoryName, const ExecutionContextHandle& executionContextHandle)
{
    CScriptHandle handle = CScriptHandle();

    asIScriptObject* object = createScriptObject(moduleHandle, objectName, factoryName, executionContextHandle);

    handle.Set(object, object->GetObjectType());

    object->Release();

    return handle;
}

asIScriptObject* ScriptingEngine::createScriptObject(const ModuleHandle& moduleHandle, const std::string& objectName, const std::string& factoryName, const ExecutionContextHandle& executionContextHandle)
{
    auto module = moduleData_[moduleHandle].module;

    asITypeInfo* type = module->GetTypeInfoByDecl(objectName.c_str());
    if (type == nullptr)
    {
        throw InvalidArgumentException(std::string("Object type with name '") + objectName + "' doesn't exist.");
    }

    asIScriptFunction* factory = type->GetFactoryByDecl(factoryName.c_str());
    if (type == nullptr)
    {
        throw InvalidArgumentException(std::string("Factory with name '") + factoryName + "' doesn't exist for object type with name '" + objectName + "'.");
    }

    auto context = getContext(executionContextHandle);

    asIScriptObject* obj = callFunctionWithReturnValue(context, factory);

    return obj;
}

void ScriptingEngine::setArguments(asIScriptContext* context, ParameterList& arguments) const
{
	int32 r = 0;

	for ( size_t i = 0; i < arguments.size(); ++i)
	{
		switch (arguments[i].type())
	    {
	        case ParameterType::TYPE_BOOL:
	            r = context->SetArgByte(static_cast<asUINT>(i), arguments[i].value<bool>());
	            break;

	        case ParameterType::TYPE_INT8:
	            r = context->SetArgByte(static_cast<asUINT>(i), arguments[i].value<int8>());
	            break;

			case ParameterType::TYPE_UINT8:
			    r = context->SetArgByte(static_cast<asUINT>(i), arguments[i].value<uint8>());
	            break;

	        case ParameterType::TYPE_INT16:
				r = context->SetArgWord(static_cast<asUINT>(i), arguments[i].value<int16>());
	            break;

	        case ParameterType::TYPE_UINT16:
	            r = context->SetArgWord(static_cast<asUINT>(i), arguments[i].value<uint16>());
	            break;

	        case ParameterType::TYPE_INT32:
				r = context->SetArgDWord(static_cast<asUINT>(i), arguments[i].value<int32>());
	            break;

	        case ParameterType::TYPE_UINT32:
	            r = context->SetArgDWord(static_cast<asUINT>(i), arguments[i].value<uint32>());
	            break;

	        case ParameterType::TYPE_INT64:
				r = context->SetArgQWord(static_cast<asUINT>(i), arguments[i].value<int64>());
	            break;

	        case ParameterType::TYPE_UINT64:
	            r = context->SetArgQWord(static_cast<asUINT>(i), arguments[i].value<uint64>());
	            break;

	        case ParameterType::TYPE_FLOAT32:
	            r = context->SetArgFloat(static_cast<asUINT>(i), arguments[i].value<float32>());
	            break;

	        case ParameterType::TYPE_FLOAT64:
	            r = context->SetArgDouble(static_cast<asUINT>(i), arguments[i].value<float64>());
	            break;

	        case ParameterType::TYPE_OBJECT_REF:
				r = context->SetArgAddress(static_cast<asUINT>(i), arguments[i].pointer());
	            break;

	        case ParameterType::TYPE_OBJECT_VAL:
				r = context->SetArgObject(static_cast<asUINT>(i), arguments[i].pointer());
	            break;

	        default:
				throw Exception("Unknown parameter type.");
				break;
		}

		assertNoAngelscriptError(r);
	}
}

asIScriptObject* ScriptingEngine::callFunctionWithReturnValue(asIScriptContext* context, asIScriptFunction* function)
{
    assert(function->GetParamCount() == 0);

    if (context->GetState() == asEContextState::asEXECUTION_ACTIVE)
    {
        int32 r = context->PushState();
        assertNoAngelscriptError(r);
    }

    int32 r = context->Prepare(function);
    assertNoAngelscriptError(r);

    debugger_->prepare(context);

    r = context->Execute();

    if ( r != asEXECUTION_FINISHED )
    {
        std::string msg = std::string();

        // The execution didn't complete as expected. Determine what happened.
        if ( r == asEXECUTION_EXCEPTION )
        {
            // An exception occurred, let the script writer know what happened so it can be corrected.
            throw Exception(detail::format("ScriptEngine: An exception occurred: %s is not valid.", GetExceptionInfo(context, true)));
        }

        assertNoAngelscriptError(r);
    }

    asIScriptObject* obj = *(asIScriptObject**)context->GetAddressOfReturnValue();

    obj->AddRef();

    if (context->IsNested())
    {
        int32 r = context->PopState();
        assertNoAngelscriptError(r);
    }

    return obj;
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptFunction* function, asIScriptObject* object)
{
	assert(function->GetParamCount() == 0);

	if (context->GetState() == asEContextState::asEXECUTION_ACTIVE)
	{
		int32 r = context->PushState();
		assertNoAngelscriptError(r);
	}

	int32 r = context->Prepare(function);
	assertNoAngelscriptError(r);

    debugger_->prepare(context);

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

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptFunction* function, asIScriptObject* object, ParameterList& arguments)
{
	assert(function->GetParamCount() == arguments.size());

	if (context->GetState() == asEContextState::asEXECUTION_ACTIVE)
	{
		int32 r = context->PushState();
		assertNoAngelscriptError(r);
	}

	int32 r = context->Prepare(function);
	assertNoAngelscriptError(r);

    debugger_->prepare(context);

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

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptFunction* function)
{
	callFunction(context, function, nullptr);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptFunction* function, ParameterList& arguments)
{
	callFunction(context, function, nullptr, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function)
{
	auto func = getFunctionByDecl(function, module);

	callFunction(context, func);
}

void ScriptingEngine::callFunction(asIScriptContext* context, asIScriptModule* module, const std::string& function, ParameterList& arguments)
{
	auto func = getFunctionByDecl(function, module);

	callFunction(context, func, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptFunctionHandle& scriptFunctionHandle)
{
	auto function = static_cast<asIScriptFunction*>(scriptFunctionHandle.get());

	callFunction(context, function);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptFunctionHandle& scriptFunctionHandle, ParameterList& arguments)
{
	auto function = static_cast<asIScriptFunction*>(scriptFunctionHandle.get());

	callFunction(context, function, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ModuleHandle& moduleHandle, const std::string& function)
{
	auto module = moduleData_[moduleHandle].module;
	callFunction(context, module, function);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ModuleHandle& moduleHandle, const std::string& function, ParameterList& arguments)
{
	auto module = moduleData_[moduleHandle].module;
	callFunction(context, module, function, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function)
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());
	auto objectFunction = getMethod(scriptObjectHandle, function);

	callFunction(context, objectFunction, object);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const std::string& function, ParameterList& arguments)
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());

	auto objectFunction = getMethod(scriptObjectHandle, function);

	callFunction(context, objectFunction, object, arguments);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle)
{
	auto object = static_cast<asIScriptObject*>(scriptObjectHandle.get());
	auto objectFunction = static_cast<asIScriptFunction*>(scriptObjectFunctionHandle.get());

	callFunction(context, objectFunction, object);
}

void ScriptingEngine::callFunction(asIScriptContext* context, const ScriptObjectHandle& scriptObjectHandle, const ScriptObjectFunctionHandle& scriptObjectFunctionHandle, ParameterList& arguments)
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
    auto& contextData = contextData_[executionContextHandle];

    contextData.context->Release();

	contextData_.destroy(executionContextHandle);
}

ModuleHandle ScriptingEngine::createModule(const std::string& name, const std::vector<std::string>& scriptData, const std::unordered_map<std::string, std::string>& includeOverrides)
{
	auto module = createModuleFromScripts(name, scriptData, includeOverrides);

    auto handle = moduleData_.create();
    auto& moduleData = moduleData_[handle];
    moduleData.module = module;

	return handle;
}

ModuleHandle ScriptingEngine::getModule(const std::string& name) const
{
    for (auto it = moduleData_.begin(); it != moduleData_.end(); it++)
    {
        if (name.compare(it->module->GetName()) == 0)
        {
            return it.handle();
        }
    }

    return ModuleHandle();
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
		throw Exception("Type with name " + name + " not found.");
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

void ScriptingEngine::tick(const float32 delta)
{
    engine_->GarbageCollect();
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
		LOG_TRACE(logger_, "Destroying module with name '%s'", m.module->GetName())
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

IScriptingEngineDebugger* ScriptingEngine::debugger()
{
    return debugger_.get();
}

// Implement a simple message callback function
void ScriptingEngine::MessageCallback(const asSMessageInfo* msg, void* param)
{
	if ( msg->type == asMSGTYPE_WARNING )
	{
		LOG_WARN(logger_, "%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
	}
	else if ( msg->type == asMSGTYPE_INFORMATION )
	{
		LOG_INFO(logger_, "%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
	}
	else
	{
		LOG_ERROR(logger_, "%s (%d, %d) : %s", msg->section, msg->row, msg->col, msg->message);
		printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, "ERR", msg->message);
	}
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

void ScriptingEngine::testPrintCallstack()
{
    int num = 0;
    for (auto context : contextData_)
    {
        printf("Context %d call stack\n", num);
        for( asUINT n = 0; n < context.context->GetCallstackSize(); n++ )
        {
            asIScriptFunction *func;
            const char *scriptSection;
            int line, column;
            func = context.context->GetFunction(n);
            line = context.context->GetLineNumber(n, &column, &scriptSection);
            printf("%s:%s:%d,%d\n", scriptSection,
                   func->GetDeclaration(),
                   line, column);
        }

        ++num;
    }
}

}
}
}
