#include "as_wrapper/AsObject.h"

namespace as_wrapper
{

AsObject::AsObject(asIScriptObject* object, asITypeInfo* type, asIScriptContext* ctx) : object_(object), type_(type), ctx_(ctx)
{
}

AsObject::~AsObject()
{
}

void AsObject::callMethod(const std::string& methodDeclaration)
{
	// Obtain the function object that represents the class method
	asIScriptFunction* func = type_->GetMethodByDecl(methodDeclaration.c_str());
	
	// Prepare the context for calling the method
	ctx_->Prepare(func);
	
	// Set the object pointer
	ctx_->SetObject(object_);
	
	// Execute the call
	ctx_->Execute();
}

}
