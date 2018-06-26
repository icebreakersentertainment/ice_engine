#ifndef IMODULEFACTORY_H_
#define IMODULEFACTORY_H_

#include <memory>

#include "IModule.hpp"

#include "graphics/IGraphicsEngine.hpp"
#include "pathfinding/IPathfindingEngine.hpp"
#include "scripting/IScriptingEngine.hpp"
#include "audio/IAudioEngine.hpp"
#include "networking/INetworkingEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{

class GameEngine;

class IModuleFactory
{
public:
	virtual ~IModuleFactory()
	{
	}
	;
	
	virtual std::unique_ptr<IModule> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger,
		graphics::IGraphicsEngine* graphicsEngine,
		pathfinding::IPathfindingEngine* pathfindingEngine,
		scripting::IScriptingEngine* scriptingEngine,
		audio::IAudioEngine* audioEngine,
		networking::INetworkingEngine* networkingEngine
	) = 0;
};

}

#endif /* IMODULEFACTORY_H_ */
