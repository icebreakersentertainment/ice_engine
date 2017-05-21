#ifndef SCRIPT_FUNCTION_HANDLE_H_
#define SCRIPT_FUNCTION_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace scripting
{

class ScriptFunctionHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const ScriptFunctionHandle INVALID;
};

}
}

#endif /* SCRIPT_FUNCTION_HANDLE_H_ */
