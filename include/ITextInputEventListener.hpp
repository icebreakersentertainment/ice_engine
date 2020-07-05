#ifndef ITEXTINPUTEVENTLISTENER_HPP_
#define ITEXTINPUTEVENTLISTENER_HPP_

#include "graphics/Event.hpp"

namespace ice_engine
{

class ITextInputEventListener
{
public:
    virtual ~ITextInputEventListener()
    {
    }
    ;

    virtual bool processEvent(const graphics::TextInputEvent& event) = 0;
};

}

#endif //ITEXTINPUTEVENTLISTENER_HPP_
