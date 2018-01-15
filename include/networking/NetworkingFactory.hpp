#ifndef NETWORKINGFACTORY_H_
#define NETWORKINGFACTORY_H_

#include <memory>

#include "INetworkingEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{
namespace networking
{

class NetworkingFactory
{
public:

	static std::unique_ptr<INetworkingEngine> createNetworkingEngine(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	);

private:
	NetworkingFactory();
	NetworkingFactory(const NetworkingFactory& other);
	virtual ~NetworkingFactory();
};

}

}

#endif /* NETWORKINGFACTORY_H_ */
