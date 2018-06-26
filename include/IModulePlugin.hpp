#ifndef IMODULEPLUGIN_H_
#define IMODULEPLUGIN_H_

#include <memory>

#include "IModuleFactory.hpp"

#include "IPlugin.hpp"

namespace ice_engine
{

class IModulePlugin : public IPlugin
{
public:
	virtual ~IModulePlugin()
	{
	}
	;

	virtual std::unique_ptr<IModuleFactory> createFactory() const = 0;

};

}

#endif /* IMODULEPLUGIN_H_ */
