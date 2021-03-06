#ifndef IMOUSEMOTIONEVENTLISTENER_H_
#define IMOUSEMOTIONEVENTLISTENER_H_

#include "graphics/Event.hpp"

namespace ice_engine
{

class IMouseMotionEventListener
{
public:
	virtual ~IMouseMotionEventListener()
	{
	}
	;
	
	virtual bool processEvent(const graphics::MouseMotionEvent& event) = 0;
};

}

#endif /* IMOUSEMOTIONEVENTLISTENER_H_ */
