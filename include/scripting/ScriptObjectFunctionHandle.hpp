#ifndef SCRIPT_OBJECT_FUNCTION_HANDLE_H_
#define SCRIPT_OBJECT_FUNCTION_HANDLE_H_

#include "scripting/ResourceHandle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptObjectFunctionHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const ScriptObjectFunctionHandle INVALID;
};

}
}

#endif /* SCRIPT_OBJECT_FUNCTION_HANDLE_H_ */
