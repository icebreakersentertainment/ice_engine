#include "graphics/GraphicsFactory.hpp"
#include "graphics/custom/GraphicsEngine.hpp"

namespace ice_engine
{
namespace graphics
{

GraphicsFactory::GraphicsFactory()
{
}

GraphicsFactory::GraphicsFactory(const GraphicsFactory& other)
{
}

GraphicsFactory::~GraphicsFactory()
{
}

std::unique_ptr<IGraphicsEngine> GraphicsFactory::createGraphicsEngine(
	utilities::Properties* properties,
	fs::IFileSystem* fileSystem,
	logger::ILogger* logger
)
{
	auto ptr = std::unique_ptr< IGraphicsEngine >( new custom::GraphicsEngine(properties, fileSystem, logger) );
	
	return std::move( ptr );
}

}
}
