#ifndef NETWORKIEVENTLISTENER_H_
#define NETWORKIEVENTLISTENER_H_

#include "Event.hpp"

namespace ice_engine
{
namespace networking
{

class IEventListener
{
public:
	virtual ~IEventListener()
	{
	}
	;
	
	virtual bool processEvent(const ConnectEvent& event) = 0;
	virtual bool processEvent(const DisconnectEvent& event) = 0;
	virtual bool processEvent(const MessageEvent& event) = 0;
};

}
}

#endif /* NETWORKIEVENTLISTENER_H_ */
