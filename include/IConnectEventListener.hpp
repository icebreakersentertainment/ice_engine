#ifndef ICONNECTEVENTLISTENER_H_
#define ICONNECTEVENTLISTENER_H_

#include "networking/Event.hpp"

namespace ice_engine
{

class IConnectEventListener
{
public:
	virtual ~IConnectEventListener()
	{
	}
	;
	
	virtual bool processEvent(const networking::ConnectEvent& event) = 0;
};

}

#endif /* ICONNECTEVENTLISTENER_H_ */
