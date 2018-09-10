#ifndef SCRIPT_HANDLE_H_
#define SCRIPT_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace scripting
{

class ScriptHandle : public handles::Handle<ScriptHandle>
{
public:
	using handles::Handle<ScriptHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::scripting::ScriptHandle>
	{
		std::size_t operator()(const ice_engine::scripting::ScriptHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* SCRIPT_HANDLE_H_ */
