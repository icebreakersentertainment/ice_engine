#ifndef ISCRIPTINGENGINEBINDINGPLUGIN_H_
#define ISCRIPTINGENGINEBINDINGPLUGIN_H_

#include <memory>

#include "IScriptingEngineBindingFactory.hpp"

#include "IPlugin.hpp"

namespace ice_engine
{

class IScriptingEngineBindingPlugin : public IPlugin
{
public:
	virtual ~IScriptingEngineBindingPlugin()
	{
	}
	;

	virtual std::unique_ptr<IScriptingEngineBindingFactory> createFactory() const = 0;

};

}

#endif /* ISCRIPTINGENGINEBINDINGPLUGIN_H_ */
