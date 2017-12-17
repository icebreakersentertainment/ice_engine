#ifndef IEVENTLISTENER_H_
#define IEVENTLISTENER_H_

#include "Event.hpp"

namespace ice_engine
{
namespace graphics
{

class IEventListener
{
public:
	virtual ~IEventListener()
	{
	}
	;
	
	virtual bool processEvent(const Event& event) = 0;
};

}

}

#endif /* IEVENTLISTENER_H_ */
