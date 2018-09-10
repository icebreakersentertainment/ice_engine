#ifndef SCRIPT_FUNCTION_HANDLE_H_
#define SCRIPT_FUNCTION_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace ice_engine
{
namespace scripting
{

class ScriptFunctionHandle : public handles::PointerHandle<ScriptFunctionHandle>
{
public:
	using handles::PointerHandle<ScriptFunctionHandle>::PointerHandle;
	
	static const ScriptFunctionHandle INVALID;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::scripting::ScriptFunctionHandle>
	{
		std::size_t operator()(const ice_engine::scripting::ScriptFunctionHandle& k) const noexcept
		{
			return hash<const void*>{}(k.get());
		}
	};
}

#endif /* SCRIPT_FUNCTION_HANDLE_H_ */
