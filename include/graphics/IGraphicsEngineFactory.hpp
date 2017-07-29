#ifndef IGRAPHICSENGINEFACTORY_H_
#define IGRAPHICSENGINEFACTORY_H_

#include <memory>

#include "IGraphicsEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace hercules
{
namespace graphics
{

class IGraphicsEngineFactory
{
public:
	virtual ~IGraphicsEngineFactory()
	{
	}
	;
	
	virtual std::unique_ptr<IGraphicsEngine> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	) = 0;
};

}
}

#endif /* IGRAPHICSENGINEFACTORY_H_ */
