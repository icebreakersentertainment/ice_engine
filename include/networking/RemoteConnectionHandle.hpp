#ifndef REMOTE_CONNECTION_HANDLE_H_
#define REMOTE_CONNECTION_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace networking
{

class RemoteConnectionHandle : public handles::Handle<RemoteConnectionHandle>
{
public:
	using handles::Handle<RemoteConnectionHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::networking::RemoteConnectionHandle>
	{
		std::size_t operator()(const ice_engine::networking::RemoteConnectionHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* REMOTE_CONNECTION_HANDLE_H_ */
