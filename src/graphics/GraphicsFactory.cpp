#include "graphics/GraphicsFactory.hpp"
#include "graphics/bgfx/GraphicsEngine.hpp"

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
	uint32 width,
	uint32 height,
	utilities::Properties* properties,
	fs::IFileSystem* fileSystem,
	logger::ILogger* logger
)
{
	auto ptr = std::unique_ptr< IGraphicsEngine >( new bgfx::GraphicsEngine(width, height, properties, fileSystem, logger) );
	
	return std::move( ptr );
}

}
}
