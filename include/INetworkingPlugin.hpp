#ifndef INETWORKINGPLUGIN_H_
#define INETWORKINGPLUGIN_H_

#include <memory>

#include "networking/INetworkingEngineFactory.hpp"

#include "IPlugin.hpp"

namespace ice_engine
{

class INetworkingPlugin : public virtual IPlugin
{
public:
	virtual ~INetworkingPlugin()
	{
	}
	;

	virtual std::unique_ptr<networking::INetworkingEngineFactory> createFactory() const = 0;

};

}

#endif /* INETWORKINGPLUGIN_H_ */
