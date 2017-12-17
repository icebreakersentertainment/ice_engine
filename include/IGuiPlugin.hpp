#ifndef IGUIPLUGIN_H_
#define IGUIPLUGIN_H_

#include <memory>

#include "graphics/gui/IGuiFactory.hpp"

#include "IPlugin.hpp"

namespace ice_engine
{

class IGuiPlugin : public IPlugin
{
public:
	virtual ~IGuiPlugin()
	{
	}
	;

	virtual std::unique_ptr<graphics::gui::IGuiFactory> createFactory() const = 0;

};

}

#endif /* IGUIPLUGIN_H_ */
