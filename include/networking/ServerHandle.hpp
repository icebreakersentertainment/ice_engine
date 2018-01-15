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

#endif /* SERVER_HANDLE_H_ */
