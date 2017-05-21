#ifndef IMOUSEWHEELEVENTLISTENER_H_
#define IMOUSEWHEELEVENTLISTENER_H_

#include "graphics/Event.hpp"

namespace hercules
{

class IMouseWheelEventListener
{
public:
	virtual ~IMouseWheelEventListener()
	{
	}
	;
	
	virtual bool processEvent(const graphics::MouseWheelEvent& event) = 0;
};

}

#endif /* IMOUSEWHEELEVENTLISTENER_H_ */
