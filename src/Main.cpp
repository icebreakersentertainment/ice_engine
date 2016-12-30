#include "Main.hpp"

#include "Game.hpp"

Main::Main()
{
}

Main::~Main()
{
}

int main()
{
	game::Game game;
	
	game.setMainScript( std::string("Main"), std::string("Main.as") );
	
	game.run();

	return 0;
}

