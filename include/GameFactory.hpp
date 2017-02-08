#ifndef GAMEFACTORY_H_
#define GAMEFACTORY_H_

#include <memory>

#include "IGameEngine.hpp"
#include "utilities/Properties.hpp"
#include "logger/ILogger.hpp"

namespace hercules
{

class GameFactory
{
public:

	static std::unique_ptr<IGameEngine> createGameEngine(
		std::unique_ptr<utilities::Properties> properties,
		std::unique_ptr<hercules::logger::ILogger> logger
	);

private:
	GameFactory();
	GameFactory(const GameFactory& other);
	virtual ~GameFactory();
};

}

#endif /* GAMEFACTORY_H_ */
