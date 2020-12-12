#ifndef IKEYBOARDEVENTLISTENER_H_
#define IKEYBOARDEVENTLISTENER_H_

#include "graphics/Event.hpp"

namespace ice_engine
{

class IKeyboardEventListener
{
public:
	virtual ~IKeyboardEventListener() = default;
	
	virtual bool processEvent(const graphics::KeyboardEvent& event) = 0;
};

}

#endif /* IKEYBOARDEVENTLISTENER_H_ */
