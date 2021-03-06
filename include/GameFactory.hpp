#ifndef GAMEFACTORY_H_
#define GAMEFACTORY_H_

#include <memory>

#include "GameEngine.hpp"
#include "utilities/Properties.hpp"
#include "IPluginManager.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{

class GameFactory
{
public:

	static std::unique_ptr<GameEngine> createGameEngine(
		std::unique_ptr<utilities::Properties> properties,
        std::unique_ptr<fs::IFileSystem> fileSystem,
		std::unique_ptr<ice_engine::IPluginManager> pluginManager,
		std::unique_ptr<ice_engine::logger::ILogger> logger
	);

private:
	GameFactory();
	GameFactory(const GameFactory& other);
	virtual ~GameFactory();
};

}

#endif /* GAMEFACTORY_H_ */
