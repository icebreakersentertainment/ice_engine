#ifndef IBUTTON_H_
#define IBUTTON_H_

#include <functional>

#include "IComponent.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IButton : public virtual IComponent
{
public:
	virtual ~IButton()
	{
	}
	;
	
	virtual void setLabel(const std::string& label) = 0;
	virtual const std::string& getLabel() const  = 0;
	
	virtual void setCallback(std::function<void()>& callback) = 0;
};

}
}
}

#endif /* IBUTTON_H_ */
