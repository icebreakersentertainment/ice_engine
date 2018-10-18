#ifndef IPATHFINDINGPLUGIN_H_
#define IPATHFINDINGPLUGIN_H_

#include <memory>

#include "pathfinding/IPathfindingEngineFactory.hpp"

#include "IPlugin.hpp"

namespace ice_engine
{

class IPathfindingPlugin : public virtual IPlugin
{
public:
	virtual ~IPathfindingPlugin()
	{
	}
	;

	virtual std::unique_ptr<pathfinding::IPathfindingEngineFactory> createFactory() const = 0;

};

}

#endif /* IPATHFINDINGPLUGIN_H_ */
