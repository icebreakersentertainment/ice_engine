#ifndef IPLUGINMANAGER_H_
#define IPLUGINMANAGER_H_

#include <vector>
#include <memory>

#include "IGuiPlugin.hpp"

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

};

}

#endif /* IPLUGINMANAGER_H_ */
