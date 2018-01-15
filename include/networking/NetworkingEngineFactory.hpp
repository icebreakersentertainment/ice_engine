#ifndef NETWORKINGENGINEFACTORY_H_
#define NETWORKINGENGINEFACTORY_H_

#include "INetworkingEngineFactory.hpp"

namespace ice_engine
{
namespace networking
{

class NetworkingEngineFactory : public INetworkingEngineFactory
{
public:
	NetworkingEngineFactory();
	virtual ~NetworkingEngineFactory();
	
	virtual std::unique_ptr<INetworkingEngine> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	) override;

private:
	NetworkingEngineFactory(const NetworkingEngineFactory& other);
};

}

}

#endif /* NETWORKINGENGINEFACTORY_H_ */
