#ifndef IDISCONNECTEVENTLISTENER_H_
#define IDISCONNECTEVENTLISTENER_H_

#include "networking/Event.hpp"

namespace ice_engine
{

class IDisconnectEventListener
{
public:
	virtual ~IDisconnectEventListener()
	{
	}
	;
	
	virtual bool processEvent(const networking::DisconnectEvent& event) = 0;
};

}

#endif /* IDISCONNECTEVENTLISTENER_H_ */
