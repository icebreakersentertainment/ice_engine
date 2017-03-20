#include "graphics/GraphicsFactory.hpp"
//#include "graphics/bgfx/GraphicsEngine.hpp"
#include "graphics/custom/GraphicsEngine.hpp"

namespace hercules
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
