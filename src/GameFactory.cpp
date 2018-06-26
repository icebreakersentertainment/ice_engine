#include "GameFactory.hpp"
#include "GameEngine.hpp"

namespace ice_engine
{

GameFactory::GameFactory()
{
}

GameFactory::GameFactory(const GameFactory& other)
{
}

GameFactory::~GameFactory()
{
}

std::unique_ptr<GameEngine> GameFactory::createGameEngine(
	std::unique_ptr<utilities::Properties> properties,
	std::unique_ptr<ice_engine::IPluginManager> pluginManager,
	std::unique_ptr<ice_engine::logger::ILogger> logger
)
{
	auto ptr = std::unique_ptr< GameEngine >( new GameEngine(std::move(properties), std::move(pluginManager), std::move(logger)) );
	
	return std::move( ptr );
}

}
