#ifndef ASOBJECT_H_
#define ASOBJECT_H_

#include <string>

#include <angelscript/angelscript.h>

namespace as_wrapper
{

class AsObject
{
public:
	AsObject(asIScriptObject* object, asIObjectType* type, asIScriptContext* ctx);
	virtual ~AsObject();
	
	void callMethod(const std::string& methodDeclaration);
private:
	asIScriptObject* object_;
	asIObjectType* type_;
	asIScriptContext* ctx_;
};

}

#endif /* ASOBJECT_H_ */
