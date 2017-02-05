#include <iostream>

#include "Main.hpp"

#include "fs/FileSystem.hpp"
#include "utilities/Properties.hpp"
#include "GameFactory.hpp"

Main::Main()
{
}

Main::~Main()
{
}

int main()
{
	// Try to load our config data
	std::string configData;
	try
	{
		auto fileSystem = fs::FileSystem();
		configData = fileSystem.readAll( std::string("settings.ini") );
	}
	catch (const std::exception& e)
	{
		std::cerr << "Unable to load settings file: " << e.what() << std::endl;
	}
	
	// Start the game engine
	try
	{
		auto properties = std::make_unique< utilities::Properties >(configData);
		auto gameEngine = hercules::GameFactory::createGameEngine(std::move(properties));
		
		gameEngine->setBootstrapScript( std::string("Main"), std::string("Main.as") );
		
		gameEngine->run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "An exception occured: " << e.what() << std::endl;
	}

	return 0;
}

