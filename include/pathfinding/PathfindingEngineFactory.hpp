#ifndef PATHFINDINGENGINEFACTORY_H_
#define PATHFINDINGENGINEFACTORY_H_

#include "pathfinding/IPathfindingEngineFactory.hpp"

namespace hercules
{
namespace pathfinding
{

class PathfindingEngineFactory : public IPathfindingEngineFactory
{
public:
	PathfindingEngineFactory();
	virtual ~PathfindingEngineFactory();
	
	virtual std::unique_ptr<IPathfindingEngine> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	) override;

private:
	PathfindingEngineFactory(const PathfindingEngineFactory& other);
};

}
}

#endif /* PATHFINDINGENGINEFACTORY_H_ */
