#ifndef ISCRIPTINGENGINEBINDINGFACTORY_H_
#define ISCRIPTINGENGINEBINDINGFACTORY_H_

#include <memory>

#include "IScriptingEngineBinding.hpp"

#include "graphics/IGraphicsEngine.hpp"
#include "pathfinding/IPathfindingEngine.hpp"
#include "physics/IPhysicsEngine.hpp"
#include "scripting/IScriptingEngine.hpp"
#include "audio/IAudioEngine.hpp"
#include "networking/INetworkingEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{

class GameEngine;

class IScriptingEngineBindingFactory
{
public:
	virtual ~IScriptingEngineBindingFactory() = default;
	
	virtual std::unique_ptr<IScriptingEngineBinding> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger,
		graphics::IGraphicsEngine* graphicsEngine,
		pathfinding::IPathfindingEngine* pathfindingEngine,
		physics::IPhysicsEngine* physicsEngine,
		scripting::IScriptingEngine* scriptingEngine,
		audio::IAudioEngine* audioEngine,
		networking::INetworkingEngine* networkingEngine,
		GameEngine* gameEngine
	) = 0;
};

}

#endif /* ISCRIPTINGENGINEBINDINGFACTORY_H_ */
