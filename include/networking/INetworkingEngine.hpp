#ifndef INETWORKINGENGINE_H_
#define INETWORKINGENGINE_H_

#include <vector>

#include "IEventListener.hpp"

#include "Event.hpp"

#include "ServerHandle.hpp"
#include "ClientHandle.hpp"
#include "RemoteConnectionHandle.hpp"

namespace ice_engine
{
namespace networking
{

class INetworkingEngine
{
public:
	virtual ~INetworkingEngine()
	{
	}
	;
	
	virtual ServerHandle createServer() = 0;
	virtual ClientHandle createClient() = 0;
	
	virtual void destroyServer(const ServerHandle& serverHandle) = 0;
	virtual void destroyClient(const ClientHandle& clientHandle) = 0;
	
	virtual void tick(const float32 delta) = 0;
	
	virtual void send(const ServerHandle& serverHandle, const std::vector<uint8>& data) = 0;
	virtual void send(const ServerHandle& serverHandle, const RemoteConnectionHandle& remoteConnectionHandle, const std::vector<uint8>& data) = 0;
	
	virtual void send(const ClientHandle& clientHandle, const std::vector<uint8>& data) = 0;
	
	virtual void processEvents() = 0;
	virtual void addEventListener(IEventListener* eventListener) = 0;
	virtual void removeEventListener(IEventListener* eventListener) = 0;
};

}

}

#endif /* INETWORKINGENGINE_H_ */
