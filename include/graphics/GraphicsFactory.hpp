#ifndef GRAPHICSFACTORY_H_
#define GRAPHICSFACTORY_H_

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IGraphicsEngine.hpp"

#include "utilities/fs/IFileSystem.hpp"

namespace graphics
{

class GraphicsFactory
{
public:

	static std::unique_ptr<IGraphicsEngine> createGraphicsEngine(glm::detail::uint32 width, glm::detail::uint32 height, utilities::fs::IFileSystem* fileSystem);

private:
	GraphicsFactory();
	GraphicsFactory(const GraphicsFactory& other);
	virtual ~GraphicsFactory();
};

}

#endif /* GRAPHICSFACTORY_H_ */
