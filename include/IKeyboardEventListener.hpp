#ifndef IKEYBOARDEVENTLISTENER_H_
#define IKEYBOARDEVENTLISTENER_H_

#include "Event.hpp"

namespace hercules
{

class IKeyboardEventListener
{
public:
	virtual ~IKeyboardEventListener()
	{
	}
	;
	
	virtual bool processEvent(const graphics::KeyboardEvent& event) = 0;
};

}

#endif /* IKEYBOARDEVENTLISTENER_H_ */
