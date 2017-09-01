#ifndef IGUIFACTORY_H_
#define IGUIFACTORY_H_

#include <memory>

#include "IGui.hpp"
#include "graphics/IGraphicsEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace hercules
{
namespace graphics
{
namespace gui
{

class IGuiFactory
{
public:
	virtual ~IGuiFactory()
	{
	}
	;
	
	virtual std::unique_ptr<IGui> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger,
		IGraphicsEngine* graphicsEngine
	) = 0;
};

}
}
}

#endif /* IGUIFACTORY_H_ */
