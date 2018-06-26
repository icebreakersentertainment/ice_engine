#ifndef IWINDOWEVENTLISTENER_H_
#define IWINDOWEVENTLISTENER_H_

#include "graphics/Event.hpp"

namespace ice_engine
{

class IWindowEventListener
{
public:
	virtual ~IWindowEventListener()
	{
	}
	;
	
	virtual bool processEvent(const graphics::WindowEvent& event) = 0;
};

}

#endif /* IWINDOWEVENTLISTENER_H_ */
