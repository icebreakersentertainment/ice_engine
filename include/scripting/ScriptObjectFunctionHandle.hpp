#ifndef SCRIPT_OBJECT_FUNCTION_HANDLE_H_
#define SCRIPT_OBJECT_FUNCTION_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace ice_engine
{
namespace scripting
{

class ScriptObjectFunctionHandle : public handles::PointerHandle<ScriptObjectFunctionHandle>
{
public:
	using handles::PointerHandle<ScriptObjectFunctionHandle>::PointerHandle;
	
	static const ScriptObjectFunctionHandle INVALID;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::scripting::ScriptObjectFunctionHandle>
	{
		std::size_t operator()(const ice_engine::scripting::ScriptObjectFunctionHandle& k) const noexcept
		{
			return hash<const void*>{}(k.get());
		}
	};
}

#endif /* SCRIPT_OBJECT_FUNCTION_HANDLE_H_ */
