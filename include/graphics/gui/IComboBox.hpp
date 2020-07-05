#ifndef ICOMBOBOX_HPP_
#define ICOMBOBOX_HPP_

#include <functional>
#include <string>

#include "IComponent.hpp"
#include "IComboBoxItem.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IComboBox : public virtual IComponent
{
public:
    virtual ~IComboBox()
    {
    }
    ;

    virtual IComboBoxItem* createItem(const std::string& label) = 0;
    virtual IComboBoxItem* getItem(const std::string& label) const = 0;
    virtual void destroy(const IComboBoxItem* comboBoxItem) = 0;

    virtual void select(IComboBoxItem* comboBoxItem) = 0;
    virtual void unselect(const IComboBoxItem* comboBoxItem) = 0;
    virtual bool selected(const IComboBoxItem* comboBoxItem) const = 0;

    virtual void setCallback(std::function<void(IComboBoxItem*)>& callback) = 0;
};

}
}
}

#endif //ICOMBOBOX_HPP_
