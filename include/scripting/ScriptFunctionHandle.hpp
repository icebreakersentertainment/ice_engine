#ifndef SCRIPT_FUNCTION_HANDLE_H_
#define SCRIPT_FUNCTION_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptFunctionHandle : public handles::PointerHandle
{
public:
	using handles::PointerHandle::PointerHandle;
	
	static const ScriptFunctionHandle INVALID;
};

}
}

#endif /* SCRIPT_FUNCTION_HANDLE_H_ */
