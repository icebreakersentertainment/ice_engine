#ifndef IMOUSEBUTTONEVENTLISTENER_H_
#define IMOUSEBUTTONEVENTLISTENER_H_

#include "graphics/Event.hpp"

namespace hercules
{

class IMouseButtonEventListener
{
public:
	virtual ~IMouseButtonEventListener()
	{
	}
	;
	
	virtual bool processEvent(const graphics::MouseButtonEvent& event) = 0;
};

}

#endif /* IMOUSEBUTTONEVENTLISTENER_H_ */
