#include <iterator>
#include <vector>

#include <boost/dll/import.hpp>

#include "PluginManager.hpp"

#include "exceptions/Exception.hpp"

#include "utilities/StringUtilities.hpp"

namespace ice_engine
{

template<class T>
auto import(const std::string& path, const std::string& name, boost::dll::load_mode::type mode)
{
	try
	{
		return boost::dll::import<T>(path, name, mode);
	}
	catch (const std::exception& e)
	{
		throw Exception("unable to load plugin " + path + ": " + e.what());
	}
}

PluginManager::PluginManager(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	)
	:
		properties_(properties),
		fileSystem_(fileSystem),
		logger_(logger)
{
	LOG_INFO(logger_, "Loading plugins.");
	
	LOG_INFO(logger_, "Loading gui plugins.");
	
	std::vector<std::string> guiPluginNames;
	utilities::explode(properties_->getStringValue("plugins.guiplugins"), ',', std::back_inserter(guiPluginNames));
	
	for (const auto& guiPluginName : guiPluginNames)
	{
		LOG_INFO(logger_, "Loading gui plugin '%s'.", guiPluginName);
		auto pluginBoostSharedPtr = import<ice_engine::IGuiPlugin>("./" + guiPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);
		
		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		auto pluginStdSharedPtr = std::shared_ptr<ice_engine::IGuiPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IGuiPlugin*){});
		
		guiPlugins_.push_back( pluginStdSharedPtr );
	}
	
	LOG_INFO(logger_, "Finished loading gui plugins.");
	
	LOG_INFO(logger_, "Loading graphics plugin.");

    const std::string graphicsPluginName = properties_->getStringValue("plugins.graphicsplugin");
	
	if (!graphicsPluginName.empty())
	{
		LOG_INFO(logger_, "Loading graphics plugin '%s'.", graphicsPluginName);
		auto pluginBoostSharedPtr = import<ice_engine::IGraphicsPlugin>("./" + graphicsPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);
		
		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		graphicsPlugin_ = std::shared_ptr<ice_engine::IGraphicsPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IGraphicsPlugin*){});
	}
	
	LOG_INFO(logger_, "Finished loading graphics plugin.");

	LOG_INFO(logger_, "Loading audio plugin.");

    const std::string audioPluginName = properties_->getStringValue("plugins.audioplugin");

	if (!audioPluginName.empty())
	{
		LOG_INFO(logger_, "Loading audio plugin '%s'.", audioPluginName);
		auto pluginBoostSharedPtr = import<ice_engine::IAudioPlugin>("./" + audioPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);

		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		audioPlugin_ = std::shared_ptr<ice_engine::IAudioPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IAudioPlugin*){});
	}

	LOG_INFO(logger_, "Finished loading audio plugin.");

	LOG_INFO(logger_, "Loading pathfinding plugin.");

	const std::string pathfindingPluginName = properties_->getStringValue("plugins.pathfindingplugin");

	if (!pathfindingPluginName.empty())
	{
		LOG_INFO(logger_, "Loading pathfinding plugin '%s'.", pathfindingPluginName);
		auto pluginBoostSharedPtr = import<ice_engine::IPathfindingPlugin>("./" + pathfindingPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);

		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		pathfindingPlugin_ = std::shared_ptr<ice_engine::IPathfindingPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IPathfindingPlugin*){});
	}

	LOG_INFO(logger_, "Finished loading pathfinding plugin.");
	
	LOG_INFO(logger_, "Loading physics plugin.");

    const std::string physicsPluginName = properties_->getStringValue("plugins.physicsplugin");

	if (!physicsPluginName.empty())
	{
		LOG_INFO(logger_, "Loading physics plugin '%s'.", physicsPluginName);
		auto pluginBoostSharedPtr = import<ice_engine::IPhysicsPlugin>("./" + physicsPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);

		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		physicsPlugin_ = std::shared_ptr<ice_engine::IPhysicsPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IPhysicsPlugin*){});
	}

	LOG_INFO(logger_, "Finished loading physics plugin.");

	LOG_INFO(logger_, "Loading networking plugin.");

	const std::string networkingPluginName = properties_->getStringValue("plugins.networkingplugin");

	if (!networkingPluginName.empty())
	{
		LOG_INFO(logger_, "Loading networking plugin '%s'.", networkingPluginName);
		auto pluginBoostSharedPtr = import<ice_engine::INetworkingPlugin>("./" + networkingPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);

		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		networkingPlugin_ = std::shared_ptr<ice_engine::INetworkingPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::INetworkingPlugin*){});
	}

	LOG_INFO(logger_, "Finished loading networking plugin.");

	LOG_INFO(logger_, "Loading module plugins.");
	
	std::vector<std::string> modulePluginNames;
	utilities::explode(properties_->getStringValue("plugins.moduleplugins"), ',', std::back_inserter(modulePluginNames));
	
	for (const auto& modulePluginName : modulePluginNames)
	{
		LOG_INFO(logger_, "Loading module plugin '%s'.", modulePluginName);
		auto pluginBoostSharedPtr = import<ice_engine::IModulePlugin>("./" + modulePluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);
		
		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		auto pluginStdSharedPtr = std::shared_ptr<ice_engine::IModulePlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IModulePlugin*){});
		
		modulePlugins_.push_back( pluginStdSharedPtr );
	}
	
	LOG_INFO(logger_, "Finished loading module plugins.");
	
	LOG_INFO(logger_, "Loading scripting engine binding plugins.");

	std::vector<std::string> scriptingEngineBindingPluginNames;
	utilities::explode(properties_->getStringValue("plugins.scriptingenginebindingplugins"), ',', std::back_inserter(scriptingEngineBindingPluginNames));

	for (const auto& scriptingEngineBindingPluginName : scriptingEngineBindingPluginNames)
	{
		LOG_INFO(logger_, "Loading scripting engine binding plugin '%s'.", scriptingEngineBindingPluginName);
		auto pluginBoostSharedPtr = import<ice_engine::IScriptingEngineBindingPlugin>("./" + scriptingEngineBindingPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);

		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		auto pluginStdSharedPtr = std::shared_ptr<ice_engine::IScriptingEngineBindingPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IScriptingEngineBindingPlugin*){});

		scriptingEngineBindingPlugins_.push_back( pluginStdSharedPtr );
	}

	LOG_INFO(logger_, "Finished loading scripting engine binding plugins.");

	LOG_INFO(logger_, "Finished loading plugins.");
}

const std::vector<std::shared_ptr<IGuiPlugin>>& PluginManager::getGuiPlugins() const
{
	return guiPlugins_;
}

std::shared_ptr<IGraphicsPlugin> PluginManager::getGraphicsPlugin() const
{
	return graphicsPlugin_;
}

std::shared_ptr<IAudioPlugin> PluginManager::getAudioPlugin() const
{
	return audioPlugin_;
}

std::shared_ptr<IPathfindingPlugin> PluginManager::getPathfindingPlugin() const
{
	return pathfindingPlugin_;
}

std::shared_ptr<IPhysicsPlugin> PluginManager::getPhysicsPlugin() const
{
	return physicsPlugin_;
}

std::shared_ptr<INetworkingPlugin> PluginManager::getNetworkingPlugin() const
{
	return networkingPlugin_;
}

const std::vector<std::shared_ptr<IModulePlugin>>& PluginManager::getModulePlugins() const
{
	return modulePlugins_;
}

const std::vector<std::shared_ptr<IScriptingEngineBindingPlugin>>& PluginManager::scriptingEngineBindingPlugins() const
{
	return scriptingEngineBindingPlugins_;
}

}
