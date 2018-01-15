#ifndef NETWORK_EVENT_H_
#define NETWORK_EVENT_H_

#include <vector>

#include "Types.hpp"

#include "ClientHandle.hpp"
#include "ServerHandle.hpp"
#include "RemoteConnectionHandle.hpp"

namespace ice_engine
{
namespace networking
{

enum EventType
{
	UNKNOWN = 0,
	
	SERVERDISCONNECT,
	CLIENTDISCONNECT,
	
	SERVERCONNECT,
	SERVERCONNECTFAILED,
	CLIENTCONNECT,
	
	SERVERMESSAGE,
	CLIENTMESSAGE
};

struct GenericEvent
{
	uint32 type;
	uint32 timestamp;
	ServerHandle serverHandle;
	ClientHandle clientHandle;
	RemoteConnectionHandle remoteConnectionHandle;
};

struct ConnectEvent : public GenericEvent
{
};

struct DisconnectEvent : public GenericEvent
{
};

struct MessageEvent : public GenericEvent
{
	std::vector<uint8> message;
};

}
}

#endif /* NETWORK_EVENT_H_ */
