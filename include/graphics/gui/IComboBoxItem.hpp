#ifndef ICOMBOBOXITEM_HPP_
#define ICOMBOBOXITEM_HPP_

#include <functional>
#include <string>

#include "IComponent.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IComboBoxItem
{
public:
    virtual ~IComboBoxItem() = default;

    virtual void setLabel(const std::string& label) = 0;
    virtual const std::string& label() const = 0;
};

}
}
}

#endif //ICOMBOBOXITEM_HPP_
