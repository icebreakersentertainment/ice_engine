#ifndef IMAINMENUBAR_H_
#define IMAINMENUBAR_H_

#include <string>

#include "IComponent.hpp"
#include "IMenu.hpp"

namespace ice_engine
{
namespace graphics
{
namespace gui
{

class IMainMenuBar : public virtual IComponent
{
public:
	virtual ~IMainMenuBar() = default;

	virtual IMenu* createMenu(const std::string& label) = 0;
	virtual IMenu* getMenu(const std::string& label) const  = 0;
};

}
}
}

#endif /* IMAINMENUBAR_H_ */
