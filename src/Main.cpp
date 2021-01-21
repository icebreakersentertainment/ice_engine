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
	auto logger = std::make_unique< ice_engine::logger::Logger >();

	setGlobalLogger(logger.get());

	auto fileSystem = std::make_unique< ice_engine::fs::FileSystem >();

	
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
	
	auto properties = std::make_unique< ice_engine::utilities::Properties >(configData);
	auto pluginManager = std::make_unique< ice_engine::PluginManager >(properties.get(), fileSystem.get(), logger.get());
	
	// Start the game engine
	try
	{
		auto gameEngine = ice_engine::GameFactory::createGameEngine(std::move(properties), std::move(fileSystem), std::move(pluginManager), std::move(logger));
		
		gameEngine->run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "An exception occured: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "An unknown exception occured" << std::endl;
	}
	
	return 0;
}
