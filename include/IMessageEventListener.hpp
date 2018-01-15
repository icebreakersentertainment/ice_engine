#ifndef IMESSAGEEVENTLISTENER_H_
#define IMESSAGEEVENTLISTENER_H_

#include "networking/Event.hpp"

namespace ice_engine
{

class IMessageEventListener
{
public:
	virtual ~IMessageEventListener()
	{
	}
	;
	
	virtual bool processEvent(const networking::MessageEvent& event) = 0;
};

}

#endif /* IMESSAGEEVENTLISTENER_H_ */
