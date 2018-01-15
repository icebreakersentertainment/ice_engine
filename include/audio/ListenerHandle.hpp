#ifndef LISTENER_HANDLE_H_
#define LISTENER_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace audio
{

class ListenerHandle : public handles::Handle<ListenerHandle>
{
public:
	using handles::Handle<ListenerHandle>::Handle;
};

}

}

#endif /* LISTENER_HANDLE_H_ */
