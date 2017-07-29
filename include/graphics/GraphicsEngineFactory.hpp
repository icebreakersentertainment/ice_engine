#ifndef GRAPHICSENGINEFACTORY_H_
#define GRAPHICSENGINEFACTORY_H_

#include "IGraphicsEngineFactory.hpp"

namespace hercules
{
namespace graphics
{

class GraphicsEngineFactory : public IGraphicsEngineFactory
{
public:
	GraphicsEngineFactory();
	virtual ~GraphicsEngineFactory();
	
	virtual std::unique_ptr<IGraphicsEngine> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	) override;

private:
	GraphicsEngineFactory(const GraphicsEngineFactory& other);
};

}

}

#endif /* GRAPHICSENGINEFACTORY_H_ */
