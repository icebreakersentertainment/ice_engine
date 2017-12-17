#ifndef GRAPHICSFACTORY_H_
#define GRAPHICSFACTORY_H_

#include <memory>

#include "IGraphicsEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{
namespace graphics
{

class GraphicsFactory
{
public:

	static std::unique_ptr<IGraphicsEngine> createGraphicsEngine(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	);

private:
	GraphicsFactory();
	GraphicsFactory(const GraphicsFactory& other);
	virtual ~GraphicsFactory();
};

}

}

#endif /* GRAPHICSFACTORY_H_ */
