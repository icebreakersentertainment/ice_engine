#ifndef SCRIPT_OBJECT_HANDLE_H_
#define SCRIPT_OBJECT_HANDLE_H_

#include "handles/PointerHandle.hpp"

namespace ice_engine
{
namespace scripting
{

class ScriptObjectHandle : public handles::PointerHandle<ScriptObjectHandle>
{
public:
	using handles::PointerHandle<ScriptObjectHandle>::PointerHandle;
	
	static const ScriptObjectHandle INVALID;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::scripting::ScriptObjectHandle>
	{
		std::size_t operator()(const ice_engine::scripting::ScriptObjectHandle& k) const noexcept
		{
			return hash<const void*>{}(k.get());
		}
	};
}

#endif /* SCRIPT_OBJECT_HANDLE_H_ */
