#ifndef SCRIPT_OBJECT_FUNCTION_HANDLE_H_
#define SCRIPT_OBJECT_FUNCTION_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptObjectFunctionHandle : public handles::PointerHandle
{
public:
	using handles::PointerHandle::PointerHandle;
	
	static const ScriptObjectFunctionHandle INVALID;
};

}
}

#endif /* SCRIPT_OBJECT_FUNCTION_HANDLE_H_ */
