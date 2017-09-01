#include <iostream>

#include "Main.hpp"

#include "fs/FileSystem.hpp"
#include "utilities/Properties.hpp"
#include "logger/Logger.hpp"
#include "GameFactory.hpp"

#include "PluginManager.hpp"

Main::Main()
{
}

Main::~Main()
{
}

int main()
{
	auto logger = std::make_unique< hercules::logger::Logger >();
	auto fileSystem = std::make_unique< hercules::fs::FileSystem >();
	
	
	// Try to load our config data
	std::string configData;
	try
	{
		configData = fileSystem->readAll( std::string("settings.ini") );
	}
	catch (const std::exception& e)
	{
		std::cerr << "Unable to load settings file: " << e.what() << std::endl;
	}
	
	auto properties = std::make_unique< hercules::utilities::Properties >(configData);
	auto pluginManager = std::make_unique< hercules::PluginManager >(properties.get(), fileSystem.get(), logger.get());
	
	// Start the game engine
	try
	{
		auto gameEngine = hercules::GameFactory::createGameEngine(std::move(properties), std::move(pluginManager), std::move(logger));
		
		gameEngine->run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "An exception occured: " << e.what() << std::endl;
	}

	return 0;
}
