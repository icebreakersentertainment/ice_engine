#include "graphics/GraphicsFactory.hpp"
#include "graphics/GraphicsEngine.hpp"

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
	fs::IFileSystem* fileSystem,
	logger::ILogger* logger
)
{
	auto ptr = std::unique_ptr< IGraphicsEngine >( new GraphicsEngine(width, height, fileSystem, logger) );
	
	return std::move( ptr );
}

}
}
