#include <iostream>
#include <stdio.h>

#include "scripting/angel_script/ScriptingEngine.hpp"

#define AS_USE_STLNAMES = 1
#include "scripting/angel_script/scriptstdstring/scriptstdstring.h"
//#include "scripting/angel_script/glm_bindings/Vec3.h"
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
	
	initialize();
}

ScriptingEngine::ScriptingEngine(const ScriptingEngine& other)
{
}

ScriptingEngine::~ScriptingEngine()
{
	destroy();
}

void ScriptingEngine::assertNoAngelscriptError(const int32 returnCode)
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
				throw Exception("ScriptEngine: Unknown error.");
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
	
	RegisterGlmBindings(engine_);
	
	/* The CScriptBuilder helper is an add-on that does the loading/processing
	 * of a script file
	 */
	builder_ = new CScriptBuilder();

	/**
	 * Setup the functions that are available to the scripts.
	 */
	// Register the function(s) that we want the scripts to call
	r = engine_->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(scripting::angel_script::ScriptingEngine::print), asCALL_CDECL);
	assertNoAngelscriptError(r);
	
	r = engine_->RegisterGlobalFunction("void println(const string &in)", asFUNCTION(scripting::angel_script::ScriptingEngine::println), asCALL_CDECL);
	assertNoAngelscriptError(r);

	/**
	 * Load all of the scripts.
	 */
	//this->loadScripts();

	// initialize context to nullptr
	ctx_ = nullptr;
}

void ScriptingEngine::loadScripts()
{
	// discard any modules that already exist
	this->discardModules();

	// load modules/scripts
	this->startNewModule("test");
	
	//this->addScript("test.as");
	//this->addScript("test2.as");
	this->buildModule();
	
	logger_->debug( "All scripts loaded successfully!" );
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

void ScriptingEngine::loadScript(const std::string& name, const std::string& filename)
{
	// load modules/scripts
	startNewModule(name);
	
	addScript(filename);
	buildModule();
}

void ScriptingEngine::loadScripts(const std::string& directory)
{
	// discard any modules that already exist
	discardModules();

	// load modules/scripts
	startNewModule("test");
	
	//addScript("test.as");
	buildModule();
}

void ScriptingEngine::unloadScripts()
{
}

void ScriptingEngine::runScript(const std::string& filename, const std::string& function)
{
	loadScript(ScriptingEngine::ONE_TIME_RUN_SCRIPT_MODULE_NAME, filename);
	initContext(function, ONE_TIME_RUN_SCRIPT_MODULE_NAME);
	run();
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
	if ( ctx_ != nullptr )
	{
		ctx_->Release();
	}
	
	// Clean up
	engine_->Release();
	delete builder_;
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

void ScriptingEngine::initContext(const std::string& function, const std::string& module)
{
	// error check
	if ( function.length() > 150 )
	{
		throw Exception("ScriptEngine: Function length is too long.");
	}
	
	logger_->debug( "Releasing old context." );
	releaseContext();
	logger_->debug( "Creating new context." );
	ctx_ = engine_->CreateContext();

	logger_->debug( "Finding the module that is to be used." );
	
	// Find the function that is to be called.
	asIScriptModule* mod = engine_->GetModule( module.c_str() );
	if ( mod == nullptr )
	{
		std::string msg = std::string();

		if ( module.length() > 80 )
		{
			msg = std::string("Module name too long.");
		}
		else
		{
			msg = std::string("Unable to locate the specified module: ");
			msg += module;
		}

		throw Exception("ScriptEngine: " + msg);
	}
	
	logger_->debug( "Getting function by the declaration." );
	
	asIScriptFunction* func = mod->GetFunctionByDecl( function.c_str() );
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

	logger_->debug( "Preparing function: " + function);
	ctx_->Prepare(func);

	if ( function.length() > 80 )
	{
		throw Exception("ScriptEngine: Preparing function: Function name too long.");
	}
}

void ScriptingEngine::setArgDWord(asUINT arg, asDWORD value)
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	ctx_->SetArgDWord(arg, value);
}

void ScriptingEngine::setArgQWord(asUINT arg, asQWORD value)
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	ctx_->SetArgQWord(arg, value);
}

void ScriptingEngine::setArgFloat(asUINT arg, const float32 value)
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	ctx_->SetArgFloat(arg, value);
}

void ScriptingEngine::setArgDouble(asUINT arg, const float64 value)
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	ctx_->SetArgDouble(arg, value);
}

void ScriptingEngine::setArgAddress(asUINT arg, void* addr)
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	ctx_->SetArgAddress(arg, addr);
}

void ScriptingEngine::setArgByte(asUINT arg, asBYTE value)
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	ctx_->SetArgByte(arg, value);
}

void ScriptingEngine::setArgObject(asUINT arg, void* obj)
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	ctx_->SetArgObject(arg, obj);
}

void ScriptingEngine::setArgWord(asUINT arg, asWORD value)
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	ctx_->SetArgWord(arg, value);
}

void ScriptingEngine::run()
{
	// error check
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}

	logger_->debug( "Executing function." );
	int32 r = ctx_->Execute();
	
	if ( r != asEXECUTION_FINISHED )
	{
		std::string msg = std::string();
		
		// The execution didn't complete as expected. Determine what happened.
		if ( r == asEXECUTION_EXCEPTION )
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			msg = std::string("An exception occurred: ");
			msg += std::string(ctx_->GetExceptionString());
			throw Exception("ScriptEngine: " + msg);
		}
		
		assertNoAngelscriptError(r);
	}
}

asDWORD ScriptingEngine::getReturnDWord()
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	return ctx_->GetReturnDWord();
}

asQWORD ScriptingEngine::getReturnQWord()
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	return ctx_->GetReturnQWord();
}

float32 ScriptingEngine::getReturnFloat()
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	return ctx_->GetReturnFloat();
}

float64 ScriptingEngine::getReturnDouble()
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	return ctx_->GetReturnDouble();
}

void* ScriptingEngine::getReturnAddress()
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	return ctx_->GetReturnAddress();
}

asBYTE ScriptingEngine::getReturnByte()
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	return ctx_->GetReturnByte();
}

void* ScriptingEngine::getReturnObject()
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	return ctx_->GetReturnObject();
}

asWORD ScriptingEngine::getReturnWord()
{
	if ( ctx_ == nullptr )
	{
		throw Exception("ScriptEngine: Context is null.");
	}
	
	return ctx_->GetReturnWord();
}

void ScriptingEngine::releaseContext()
{
	if ( ctx_ != nullptr )
	{
		ctx_->Release();
		ctx_ = nullptr;
	}
}

void ScriptingEngine::startNewModule(const std::string& module)
{
	int32 r = builder_->StartNewModule(engine_, module.c_str());
	assertNoAngelscriptError(r);
}

void ScriptingEngine::addScript(const std::string& script)
{
	// error check
	if ( script.length() > 150 )
	{
		throw Exception("ScriptEngine: Script filename too long.");
	}

	// TODO: maybe make this a constant or something..?  Or make the directory an instance variable?
	//std::string file = fs::current_path().string() + std::string("/scripts/") + script;
	std::string file = std::string("../data/scripts/") + script;
	
	int32 r = builder_->AddSectionFromFile(file.c_str());
	assertNoAngelscriptError(r);
}

void ScriptingEngine::buildModule()
{
	int32 r = builder_->BuildModule();
	assertNoAngelscriptError(r);
}

void ScriptingEngine::discardModule(const std::string& name)
{
	int32 r = engine_->DiscardModule( name.c_str() );
	assertNoAngelscriptError(r);
}

void ScriptingEngine::discardModules()
{
	discardModule( std::string("test"));
}

AsObject* ScriptingEngine::createAsObject(const std::string& moduleName, const std::string& className)
{
	// Get the object type
	asIScriptModule* module = engine_->GetModule(moduleName.c_str());
	asITypeInfo* type = engine_->GetTypeInfoById( module->GetTypeIdByDecl(className.c_str()) );
	
	// Get the factory function from the object type
	const std::string declaration = className + " @" + className + "()";
	asIScriptFunction* factory = type->GetFactoryByDecl(declaration.c_str());
	
	auto ctx = engine_->CreateContext();
	
	// Prepare the context to call the factory function
	ctx->Prepare(factory);
	
	// Execute the call
	ctx->Execute();
	
	// Get the object that was created
	asIScriptObject* object = *(asIScriptObject**)ctx->GetAddressOfReturnValue();
	
	// If you're going to store the object you must increase the reference,
	// otherwise it will be destroyed when the context is reused or destroyed.
	object->AddRef();
	
	return new AsObject(object, type, ctx);
}

}
}
}
