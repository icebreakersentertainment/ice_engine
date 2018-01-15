#ifndef IGRAPHICSPLUGIN_H_
#define IGRAPHICSPLUGIN_H_

#include <memory>

#include "graphics/IGraphicsEngineFactory.hpp"

#include "IPlugin.hpp"

namespace ice_engine
{

class IGraphicsPlugin : public virtual IPlugin
{
public:
	virtual ~IGraphicsPlugin()
	{
	}
	;

	virtual std::unique_ptr<graphics::IGraphicsEngineFactory> createFactory() const = 0;

};

}

#endif /* IGRAPHICSPLUGIN_H_ */
