#ifndef SCRIPT_OBJECT_FUNCTION_HANDLE_H_
#define SCRIPT_OBJECT_FUNCTION_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptObjectFunctionHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const ScriptObjectFunctionHandle INVALID;
};

}
}

#endif /* SCRIPT_OBJECT_FUNCTION_HANDLE_H_ */
