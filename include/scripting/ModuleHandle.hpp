#ifndef MODULE_HANDLE_H_
#define MODULE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace scripting
{

class ModuleHandle : public handles::Handle<ModuleHandle>
{
public:
	using handles::Handle<ModuleHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::scripting::ModuleHandle>
	{
		std::size_t operator()(const ice_engine::scripting::ModuleHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* MODULE_HANDLE_H_ */
