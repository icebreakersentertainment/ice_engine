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
	try
	{
		game::Game game;
		
		game.setMainScript( std::string("Main"), std::string("Main.as") );
		
		game.run();
	}
	catch (const std::exception& e)
	{
		std::cout << "An exception occured: " << e.what() << std::endl;
	}

	return 0;
}

