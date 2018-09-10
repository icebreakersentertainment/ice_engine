#ifndef SERVER_HANDLE_H_
#define SERVER_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace networking
{

class ServerHandle : public handles::Handle<ServerHandle>
{
public:
	using handles::Handle<ServerHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::networking::ServerHandle>
	{
		std::size_t operator()(const ice_engine::networking::ServerHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* SERVER_HANDLE_H_ */
