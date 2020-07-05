#ifndef ITEXTFIELD_H_
#define ITEXTFIELD_H_

#include <string>
#include <functional>

#include "IComponent.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class ITextField : public virtual IComponent
{
public:
    virtual ~ITextField() = default;

    virtual void setText(const std::string& text) = 0;
    virtual std::string getText() const  = 0;
};

}
}
}

#endif //ITEXTFIELD_H_
