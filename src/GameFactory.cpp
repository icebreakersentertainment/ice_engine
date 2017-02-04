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

std::unique_ptr<IGameEngine> GameFactory::createGameEngine()
{
	auto ptr = std::unique_ptr< IGameEngine >( new GameEngine() );
	
	return std::move( ptr );
}

}
