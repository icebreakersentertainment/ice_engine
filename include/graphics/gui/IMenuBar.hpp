#ifndef IMENUBAR_H_
#define IMENUBAR_H_

#include <string>

#include "IComponent.hpp"
#include "IMenu.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IMenuBar : public virtual IComponent
{
public:
	virtual ~IMenuBar()
	{
	}
	;
	
	virtual IMenu* createMenu(const std::string& label) = 0;
	virtual IMenu* getMenu(const std::string& label) const  = 0;
};

}
}
}

#endif /* IMENUBAR_H_ */
