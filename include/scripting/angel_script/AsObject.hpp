#ifndef ASOBJECT_H_
#define ASOBJECT_H_

#include <string>

#include <angelscript/angelscript.h>

namespace hercules
{
namespace scripting
{
namespace angel_script
{

class AsObject
{
public:
	AsObject(asIScriptObject* object, asITypeInfo* type, asIScriptContext* ctx);
	virtual ~AsObject();
	
	void callMethod(const std::string& methodDeclaration);
private:
	asIScriptObject* object_;
	asITypeInfo* type_;
	asIScriptContext* ctx_;
};

}
}
}

#endif /* ASOBJECT_H_ */
