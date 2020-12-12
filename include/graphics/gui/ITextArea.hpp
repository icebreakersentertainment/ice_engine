#ifndef ITEXTAREA_HPP_
#define ITEXTAREA_HPP_

#include <string>
#include <functional>

#include "IComponent.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class ITextArea : public virtual IComponent
{
public:
    virtual ~ITextArea() = default;

    virtual const std::string& text() const  = 0;
    virtual void setText(const std::string& text) = 0;

    virtual void setOnChangeCallback(std::function<void()>& onChangeCallback) = 0;
};

}
}
}

#endif //ITEXTAREA_HPP_
