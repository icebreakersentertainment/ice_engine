#include "GameFactory.hpp"
#include "GameEngine.hpp"

namespace hercules
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

std::unique_ptr<IGameEngine> GameFactory::createGameEngine(
	std::unique_ptr<utilities::Properties> properties,
	std::unique_ptr<hercules::logger::ILogger> logger
)
{
	auto ptr = std::unique_ptr< IGameEngine >( new GameEngine(std::move(properties), std::move(logger)) );
	
	return std::move( ptr );
}

}
