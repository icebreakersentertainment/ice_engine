#ifndef EXECUTION_CONTEXT_HANDLE_H_
#define EXECUTION_CONTEXT_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace scripting
{

class ExecutionContextHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const ExecutionContextHandle INVALID;
};

}
}

#endif /* EXECUTION_CONTEXT_HANDLE_H_ */
