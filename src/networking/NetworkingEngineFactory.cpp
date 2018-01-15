#include "networking/NetworkingEngineFactory.hpp"
#include "networking/yojimbo/NetworkingEngine.hpp"

namespace ice_engine
{
namespace networking
{

NetworkingEngineFactory::NetworkingEngineFactory()
{
}

NetworkingEngineFactory::NetworkingEngineFactory(const NetworkingEngineFactory& other)
{
}

NetworkingEngineFactory::~NetworkingEngineFactory()
{
}

std::unique_ptr<INetworkingEngine> NetworkingEngineFactory::create(
	utilities::Properties* properties,
	fs::IFileSystem* fileSystem,
	logger::ILogger* logger
)
{
	std::unique_ptr<INetworkingEngine> ptr = std::make_unique< yojimbo::NetworkingEngine >( properties, fileSystem, logger );
	
	return std::move( ptr );
}

}
}
