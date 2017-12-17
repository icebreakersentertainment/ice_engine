#include "pathfinding/PathfindingEngineFactory.hpp"
#include "pathfinding/recastnavigation/PathfindingEngine.hpp"

namespace ice_engine
{
namespace pathfinding
{

PathfindingEngineFactory::PathfindingEngineFactory()
{
}

PathfindingEngineFactory::PathfindingEngineFactory(const PathfindingEngineFactory& other)
{
}

PathfindingEngineFactory::~PathfindingEngineFactory()
{
}

std::unique_ptr<IPathfindingEngine> PathfindingEngineFactory::create(
	utilities::Properties* properties,
	fs::IFileSystem* fileSystem,
	logger::ILogger* logger
)
{
	auto ptr = std::unique_ptr< IPathfindingEngine >( new recastnavigation::PathfindingEngine(properties, fileSystem, logger) );
	
	return std::move( ptr );
}

}
}
