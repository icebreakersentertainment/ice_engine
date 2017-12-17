#ifndef EXECUTION_CONTEXT_HANDLE_H_
#define EXECUTION_CONTEXT_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace scripting
{

class ExecutionContextHandle : public handles::Handle<ExecutionContextHandle>
{
public:
	using handles::Handle<ExecutionContextHandle>::Handle;
};

}
}

#endif /* EXECUTION_CONTEXT_HANDLE_H_ */
