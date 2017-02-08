#include <string>
#include <iostream>
#include <stdio.h>

#include "as_wrapper/AngelScript.h"

#define AS_USE_STLNAMES = 1
#include "as_wrapper/scriptstdstring/scriptstdstring.h"

#include "as_wrapper/glm_bindings/Vec3.h"

namespace hercules
{
namespace as_wrapper
{

AngelScript::AngelScript(logger::ILogger* logger) : logger_(logger)
{
	initialize();
}

AngelScript::~AngelScript()
{
	destroy();
}

void AngelScript::initialize()
{
	engine_ = asCreateScriptEngine(ANGELSCRIPT_VERSION);
	// Set the message callback to receive information on errors in human readable form.
	int32 r = engine_->SetMessageCallback(asMETHOD(AngelScript, MessageCallback), this, asCALL_THISCALL);

	if ( r < 0 )
		assert(0);

	/* AngelScript doesn't have a built-in string type, as there is no definite standard
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
	r = engine_->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(as_wrapper::AngelScript::print), asCALL_CDECL);
	if ( r < 0 )
		assert(0);
	r = engine_->RegisterGlobalFunction("void println(const string &in)", asFUNCTION(as_wrapper::AngelScript::println), asCALL_CDECL);
	if ( r < 0 )
		assert(0);

	/**
	 * Load all of the scripts.
	 */
	//this->loadScripts();

	// initialize context to nullptr
	ctx_ = nullptr;
}

bool AngelScript::loadScripts()
{
	// discard any modules that already exist
	this->discardModules();

	// load modules/scripts
	if ( this->startNewModule("test") >= 0 )
	{
		int32 r = 0;
		r += this->addScript("test.as");
		r += this->addScript("test2.as");
		r += this->buildModule();
		if ( r < 0 )
		{
			logger_->warn( "An error occured while loading scripts!" );
			return false;
		}
		logger_->debug( "All scripts loaded successfully!" );
	}
	
	return true;
}

bool AngelScript::registerGlobalFunction(const std::string& name, const asSFuncPtr& funcPointer, asDWORD callConv, void* objForThiscall)
{
	int32 r = engine_->RegisterGlobalFunction(name.c_str(), funcPointer, callConv, objForThiscall);
	
	if ( r < 0 )
	{
		logger_->warn( "An error occured while registering global function '" + name + "'." );
		return false;
	}
	
	return true;
}

void AngelScript::registerGlobalProperty(const std::string& declaration, void* pointer)
{
	int32 r = engine_->RegisterGlobalProperty(declaration.c_str(), pointer);
	
	if ( r < 0 )
	{
		logger_->warn( "An error occured while registering global property '" + declaration + "'." );
		assert(0);
	}
}

bool AngelScript::registerClass(const std::string& name)
{
	int32 r = registerObjectType(name.c_str(), 0, asOBJ_REF);
	
	return (r == 0);
}

bool AngelScript::registerClass(const std::string& name, const std::string& classFactorySignature, const std::string& addRefSignature, const std::string& releaseRefSignature, 
	const asSFuncPtr& classFactoryFuncPointer, const asSFuncPtr& addRefFuncPointer, const asSFuncPtr& releaseRefFuncPointer)
{
	bool success = true;
	
	success = success && registerClass(name);
	success = success && registerClassFactory(name, classFactorySignature, classFactoryFuncPointer);
	success = success && registerClassAddRef(name, addRefSignature, addRefFuncPointer);
	success = success && registerClassReleaseRef(name, releaseRefSignature, releaseRefFuncPointer);
	
	return success;
}
		
bool AngelScript::registerClassFactory(const std::string& name, const std::string& classFactorySignature, const asSFuncPtr& classFactoryFuncPointer)
{
	int32 r = registerObjectBehaviour(name.c_str(), asBEHAVE_FACTORY, classFactorySignature.c_str(), classFactoryFuncPointer, asCALL_CDECL);
	
	return (r == 0);
}

bool AngelScript::registerClassAddRef(const std::string& name, const std::string& addRefSignature, const asSFuncPtr& addRefFuncPointer)
{
	int32 r = registerObjectBehaviour(name.c_str(), asBEHAVE_ADDREF, addRefSignature.c_str(), addRefFuncPointer, asCALL_THISCALL);
	
	return (r == 0);
}

bool AngelScript::registerClassReleaseRef(const std::string& name, const std::string& releaseRefSignature, const asSFuncPtr& releaseRefFuncPointer)
{
	int32 r = registerObjectBehaviour(name.c_str(), asBEHAVE_RELEASE, releaseRefSignature.c_str(), releaseRefFuncPointer, asCALL_THISCALL);
	
	return (r == 0);
}
		
bool AngelScript::registerClassMethod(const std::string& className, const std::string& methodSignature, const asSFuncPtr& funcPointer)
{
	int32 r = registerObjectMethod(className.c_str(), methodSignature.c_str(), funcPointer, asCALL_THISCALL);
	
	return (r == 0);
}

bool AngelScript::loadScript(const std::string& name, const std::string& filename)
{
	// load modules/scripts
	if ( this->startNewModule(name) >= 0 )
	{
		int32 r = 0;
		r += this->addScript(filename);
		
		r += this->buildModule();
		if ( r < 0 )
		{
			logger_->warn( "An error occured while loading script '" + filename + "'." );
			
			return false;
		}
		logger_->debug( "Script '" + filename + "' loaded successfully." );
	}
	
	return true;
}

bool AngelScript::loadScripts(const std::string& directory)
{
	// discard any modules that already exist
	this->discardModules();

	// load modules/scripts
	if ( this->startNewModule("test") >= 0 )
	{
		int32 r = 0;
		r += this->addScript("test.as");
		//r += this->addScript("test2.as");
		r += this->buildModule();
		if ( r < 0 )
		{
			logger_->debug( "An error occured while loading scripts!" );
			return false;
		}
		
		logger_->debug( "All scripts loaded successfully!" );
	}
	
	return true;
}

void AngelScript::unloadScripts()
{
}

AScriptModule* getScript(const std::string& name)
{
	
	return nullptr;
}

int32 AngelScript::registerObjectType(const std::string& obj, int32 byteSize, asDWORD flags)
{
	//logger_->debug( std::string("create angelscript wrapper1: ") + obj + " " + byteSize + " " + flags );
	if ( engine_->RegisterObjectType(obj.c_str(), byteSize, flags) < 0 )
	{
		logger_->debug( "create angelscript wrapper2." );
		// debug message

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
		
		logger_->warn( msg );
		
		return -1;
	}

	return 0;
}

int32 AngelScript::registerObjectMethod(const std::string& obj, const std::string& declaration,
									  const asSFuncPtr& funcPointer, asDWORD callConv)
{
	if ( engine_->RegisterObjectMethod(obj.c_str(), declaration.c_str(), funcPointer, callConv) < 0 )
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
		
		logger_->warn( msg );

		return -1;
	}

	return 0;
}

int32 AngelScript::registerObjectBehaviour(const std::string& obj, asEBehaviours behaviour,
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

		logger_->warn( msg );

		
		switch ( r )
		{
			case asWRONG_CONFIG_GROUP:
				logger_->warn("Error returned was: asWRONG_CONFIG_GROUP");
				break;
	
			case asINVALID_ARG:
				logger_->warn("Error returned was: asINVALID_ARG");
				break;
	
			case asNOT_SUPPORTED:
				logger_->warn("Error returned was: asNOT_SUPPORTED");
				break;
	
			case asWRONG_CALLING_CONV:
				logger_->warn("Error returned was: asWRONG_CALLING_CONV");
				break;
	
			case asINVALID_TYPE:
				logger_->warn("Error returned was: asINVALID_TYPE");
				break;
	
			case asINVALID_DECLARATION:
				logger_->warn("Error returned was: asINVALID_DECLARATION");
				break;
	
			case asILLEGAL_BEHAVIOUR_FOR_TYPE:
				logger_->warn("Error returned was: asILLEGAL_BEHAVIOUR_FOR_TYPE");
				break;
	
			case asALREADY_REGISTERED:
				logger_->warn("Error returned was: asALREADY_REGISTERED");
				break;
				
			default:
				logger_->warn("Unknown error type returned.");
				break;
		}

		return r;
	}

	return 0;
}

void AngelScript::destroy()
{
	if ( ctx_ != nullptr )
		ctx_->Release();
	// Clean up
	engine_->Release();
	delete builder_;
}

// Implement a simple message callback function
void AngelScript::MessageCallback(const asSMessageInfo* msg, void* param)
{
	std::string type = std::string("ERR ");

	if ( msg->type == asMSGTYPE_WARNING )
		type = std::string("WARN");
	else if ( msg->type == asMSGTYPE_INFORMATION )
		type = std::string("INFO");
	
	printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type.c_str(), msg->message);
}

// Print the script string to the standard output stream
void AngelScript::print(const std::string& msg)
{
	printf("%s", msg.c_str());
}

void AngelScript::println(const std::string& msg)
{
	printf("%s\n", msg.c_str());
}

bool AngelScript::initContext(const std::string& function, const std::string& module)
{
	// error check
	if ( function.length() > 150 )
	{
		logger_->warn( "Function length is too long." );
		return false;
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

		logger_->warn( msg );
		
		return false;
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

		logger_->warn( msg );

		return false;
	}

	logger_->debug( "Preparing function: " + function);
	ctx_->Prepare(func);

	if ( function.length() > 80 )
	{
		logger_->warn( std::string("Preparing function: Function name too long.") );
	}
	
	return true;
}

int32 AngelScript::setArgDWord(asUINT arg, asDWORD value)
{
	if ( ctx_ == nullptr )
		return -1;
	return ctx_->SetArgDWord(arg, value);
}

int32 AngelScript::setArgQWord(asUINT arg, asQWORD value)
{
	if ( ctx_ == nullptr )
		return -1;
	return ctx_->SetArgQWord(arg, value);
}

int32 AngelScript::setArgFloat(asUINT arg, float32 value)
{
	if ( ctx_ == nullptr )
		return -1;
	return ctx_->SetArgFloat(arg, value);
}

int32 AngelScript::setArgDouble(asUINT arg, float64 value)
{
	if ( ctx_ == nullptr )
		return -1;
	return ctx_->SetArgDouble(arg, value);
}

int32 AngelScript::setArgAddress(asUINT arg, void* addr)
{
	if ( ctx_ == nullptr )
		return -1;
	return ctx_->SetArgAddress(arg, addr);
}

int32 AngelScript::setArgByte(asUINT arg, asBYTE value)
{
	if ( ctx_ == nullptr )
		return -1;
	return ctx_->SetArgByte(arg, value);
}

int32 AngelScript::setArgObject(asUINT arg, void* obj)
{
	if ( ctx_ == nullptr )
		return -1;
	return ctx_->SetArgObject(arg, obj);
}

int32 AngelScript::setArgWord(asUINT arg, asWORD value)
{
	if ( ctx_ == nullptr )
		return -1;
	return ctx_->SetArgWord(arg, value);
}

int32 AngelScript::run()
{
	// error check
	if ( ctx_ == nullptr )
	{
		logger_->error( "Context is null!" );
		// TODO: Throw exception
		return -1;
	}

	logger_->debug( "Executing function." );
	int32 r = ctx_->Execute();
	
	if ( r != asEXECUTION_FINISHED )
	{
		// The execution didn't complete as expected. Determine what happened.
		if ( r == asEXECUTION_EXCEPTION )
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			logger_->error( "An exception occurred: " );
			logger_->error( std::string(ctx_->GetExceptionString()) );
			logger_->error( "Please correct the code and try again." );
		}
		else
		{
			logger_->error( "An exception occurred!" );
		}
		
		// TODO: Throw exception
		return -1;
	}

	logger_->debug( "Function executed successfully!" );

	return 0;
}

asDWORD AngelScript::getReturnDWord()
{
	if ( ctx_ == nullptr )
		return 0;
	return ctx_->GetReturnDWord();
}

asQWORD AngelScript::getReturnQWord()
{
	if ( ctx_ == nullptr )
		return 0;
	return ctx_->GetReturnQWord();
}

float32 AngelScript::getReturnFloat()
{
	if ( ctx_ == nullptr )
		return 0;
	return ctx_->GetReturnFloat();
}

float64 AngelScript::getReturnDouble()
{
	if ( ctx_ == nullptr )
		return 0;
	return ctx_->GetReturnDouble();
}

void* AngelScript::getReturnAddress()
{
	if ( ctx_ == nullptr )
		return 0;
	return ctx_->GetReturnAddress();
}

asBYTE AngelScript::getReturnByte()
{
	if ( ctx_ == nullptr )
		return 0;
	return ctx_->GetReturnByte();
}

void* AngelScript::getReturnObject()
{
	if ( ctx_ == nullptr )
		return 0;
	return ctx_->GetReturnObject();
}

asWORD AngelScript::getReturnWord()
{
	if ( ctx_ == nullptr )
		return 0;
	return ctx_->GetReturnWord();
}

void AngelScript::releaseContext()
{
	if ( ctx_ != nullptr )
	{
		ctx_->Release();
		ctx_ = nullptr;
	}
}

int32 AngelScript::startNewModule(const std::string& module)
{
	int32 r = builder_->StartNewModule(engine_, module.c_str());

	if ( r < 0 )
	{
		// If the code fails here it is usually because there
		// is no more memory to allocate the module
		logger_->error( "Unrecoverable error while starting a new module." );
		//updateAll("Unrecoverable error while starting a new module.");
		
		// TODO: Throw exception
		return -1;
	}
	return 0;
}

int32 AngelScript::addScript(const std::string& script)
{
	// error check
	if ( script.length() > 150 )
		return -1;

	// TODO: maybe make this a constant or something..?  Or make the directory an instance variable?
	//std::string file = fs::current_path().string() + std::string("/scripts/") + script;
	std::string file = std::string("../data/scripts/") + script;
	
	int32 r = builder_->AddSectionFromFile(file.c_str());
	if ( r < 0 )
	{
		// The builder wasn't able to load the file. Maybe the file
		// has been removed, or the wrong name was given, or some
		// preprocessing commands are incorrectly written.
		logger_->error( "Please correct the errors in the script and try again." );
		//updateAll("Please correct the errors in the script and try again.");
		
		// TODO: Throw exception
		return -1;
	}

	return 0;
}

int32 AngelScript::buildModule()
{
	int32 r = builder_->BuildModule();

	if ( r < 0 )
	{
		// An error occurred. Instruct the script writer to fix the
		// compilation errors that were listed in the output stream.
		logger_->error( "Please correct the errors in the script and try again." );
		
		// TODO: Throw exception
		return -1;
	}

	return 0;
}

int32 AngelScript::discardModule(const std::string& name)
{
	int32 r = engine_->DiscardModule( name.c_str() );

	if ( r < 0 )
	{
		logger_->warn( "Unable to discard module!" );
	}

	return r;
}

int32 AngelScript::discardModules()
{
	return discardModule( std::string("test"));
}

AsObject* AngelScript::createAsObject(const std::string& moduleName, const std::string& className)
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
