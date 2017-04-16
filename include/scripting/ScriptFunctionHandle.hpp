#ifndef SCRIPT_FUNCTION_HANDLE_H_
#define SCRIPT_FUNCTION_HANDLE_H_

#include "scripting/ResourceHandle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptFunctionHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const ScriptFunctionHandle INVALID;
};

}
}

#endif /* SCRIPT_FUNCTION_HANDLE_H_ */
