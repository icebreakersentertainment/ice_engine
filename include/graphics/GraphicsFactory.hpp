#ifndef GRAPHICSFACTORY_H_
#define GRAPHICSFACTORY_H_

#include <memory>

#include "IGraphicsEngine.hpp"

#include "fs/IFileSystem.hpp"

namespace hercules
{
namespace graphics
{

class GraphicsFactory
{
public:

	static std::unique_ptr<IGraphicsEngine> createGraphicsEngine(uint32 width, uint32 height, fs::IFileSystem* fileSystem);

private:
	GraphicsFactory();
	GraphicsFactory(const GraphicsFactory& other);
	virtual ~GraphicsFactory();
};

}

}

#endif /* GRAPHICSFACTORY_H_ */
