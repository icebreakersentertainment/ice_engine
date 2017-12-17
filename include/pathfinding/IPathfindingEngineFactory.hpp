#ifndef IPATHFINDINGENGINEFACTORY_H_
#define IPATHFINDINGENGINEFACTORY_H_

#include <memory>

#include "pathfinding/IPathfindingEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{
namespace pathfinding
{

class IPathfindingEngineFactory
{
public:
	virtual ~IPathfindingEngineFactory()
	{
	}
	;
	
	virtual std::unique_ptr<IPathfindingEngine> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	) = 0;
};

}
}

#endif /* IPATHFINDINGENGINEFACTORY_H_ */
