#ifndef EXECUTION_CONTEXT_HANDLE_H_
#define EXECUTION_CONTEXT_HANDLE_H_

#include "scripting/ResourceHandle.hpp"

namespace hercules
{
namespace scripting
{

class ExecutionContextHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const ExecutionContextHandle INVALID;
};

}
}

#endif /* EXECUTION_CONTEXT_HANDLE_H_ */
