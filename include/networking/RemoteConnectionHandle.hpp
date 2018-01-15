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

#endif /* REMOTE_CONNECTION_HANDLE_H_ */
