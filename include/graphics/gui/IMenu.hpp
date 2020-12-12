#ifndef IMENU_H_
#define IMENU_H_

#include <string>

#include "IComponent.hpp"
#include "IMenuItem.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IMenu : public virtual IComponent
{
public:
	virtual ~IMenu() = default;
	
	virtual void setLabel(const std::string& label) = 0;
	virtual const std::string& label() const  = 0;
	
	virtual IMenu* createMenu(const std::string& label) = 0;
	virtual IMenu* getMenu(const std::string& label) const  = 0;
	
	virtual void createSeparator() = 0;
	
	virtual IMenuItem* createMenuItem(const std::string& label) = 0;
};

}
}
}

#endif /* IMENU_H_ */
