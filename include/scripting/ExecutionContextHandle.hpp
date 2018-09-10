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

namespace std
{
	template <>
	struct hash<ice_engine::scripting::ExecutionContextHandle>
	{
		std::size_t operator()(const ice_engine::scripting::ExecutionContextHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* EXECUTION_CONTEXT_HANDLE_H_ */
