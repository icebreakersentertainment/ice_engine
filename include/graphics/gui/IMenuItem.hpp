#ifndef IMENUITEM_H_
#define IMENUITEM_H_

#include <string>
#include <functional>

#include "IComponent.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IMenuItem : public virtual IComponent
{
public:
	virtual ~IMenuItem() = default;

	virtual void setLabel(const std::string& label) = 0;
	virtual const std::string& label() const  = 0;

	virtual void setCallback(std::function<void()>& callback) = 0;

	virtual IMenuItem* createMenuItem(const std::string& label) = 0;
};

}
}
}

#endif /* IMENUITEM_H_ */
