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

#endif /* CLIENT_HANDLE_H_ */
