#include <iostream>

#include "Main.hpp"

#include "GameFactory.hpp"

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
		auto gameEngine = hercules::GameFactory::createGameEngine();
		
		gameEngine->setBootstrapScript( std::string("Main"), std::string("Main.as") );
		
		gameEngine->run();
	}
	catch (const std::exception& e)
	{
		std::cout << "An exception occured: " << e.what() << std::endl;
	}

	return 0;
}

