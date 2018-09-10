#ifndef IPLUGINMANAGER_H_
#define IPLUGINMANAGER_H_

#include <vector>
#include <memory>

#include "IGuiPlugin.hpp"
#include "IGraphicsPlugin.hpp"
#include "IModulePlugin.hpp"
#include "IScriptingEngineBindingPlugin.hpp"

namespace ice_engine
{

class IPluginManager
{
public:
	virtual ~IPluginManager()
	{
	}
	;

	virtual const std::vector<std::shared_ptr<IGuiPlugin>>& getGuiPlugins() const = 0;
	virtual std::shared_ptr<IGraphicsPlugin> getGraphicsPlugin() const = 0;
	virtual const std::vector<std::shared_ptr<IModulePlugin>>& getModulePlugins() const = 0;
	virtual const std::vector<std::shared_ptr<IScriptingEngineBindingPlugin>>& getScriptingEngineBindingPlugins() const = 0;

};

}

#endif /* IPLUGINMANAGER_H_ */
