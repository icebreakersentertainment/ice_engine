#ifndef INETWORKINGENGINEFACTORY_H_
#define INETWORKINGENGINEFACTORY_H_

#include <memory>

#include "INetworkingEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{
namespace networking
{

class INetworkingEngineFactory
{
public:
	virtual ~INetworkingEngineFactory()
	{
	}
	;
	
	virtual std::unique_ptr<INetworkingEngine> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	) = 0;
};

}
}

#endif /* INETWORKINGENGINEFACTORY_H_ */
