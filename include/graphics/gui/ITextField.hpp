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
    virtual const std::string& text() const  = 0;

    virtual void setOnChangeCallback(std::function<void()>& onChangeCallback) = 0;
};

}
}
}

#endif //ITEXTFIELD_H_
