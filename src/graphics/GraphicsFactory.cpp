#include "graphics/GraphicsFactory.hpp"
#include "graphics/GraphicsEngine.hpp"

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

std::unique_ptr<IGraphicsEngine> GraphicsFactory::createGraphicsEngine(glm::detail::uint32 width, glm::detail::uint32 height)
{
	auto ptr = std::unique_ptr< IGraphicsEngine >( new GraphicsEngine(width, height) );
	
	return std::move( ptr );
}

}
