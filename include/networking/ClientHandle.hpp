#ifndef CLIENT_HANDLE_H_
#define CLIENT_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace networking
{

class ClientHandle : public handles::Handle<ClientHandle>
{
public:
	using handles::Handle<ClientHandle>::Handle;
};

}

}

namespace std
{
	template <>
	struct hash<ice_engine::networking::ClientHandle>
	{
		std::size_t operator()(const ice_engine::networking::ClientHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* CLIENT_HANDLE_H_ */
