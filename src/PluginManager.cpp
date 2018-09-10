#include <iterator>
#include <vector>

#include <boost/dll/import.hpp>

#include "PluginManager.hpp"

#include "utilities/StringUtilities.hpp"

namespace ice_engine
{

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
		LOG_INFO(logger_, "Loading gui plugin '" + guiPluginName + "'.");
		auto pluginBoostSharedPtr = boost::dll::import<ice_engine::IGuiPlugin>("./" + guiPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);
		
		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		auto pluginStdSharedPtr = std::shared_ptr<ice_engine::IGuiPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IGuiPlugin*){});
		
		guiPlugins_.push_back( pluginStdSharedPtr );
	}
	
	LOG_INFO(logger_, "Finished loading gui plugins.");
	
	LOG_INFO(logger_, "Loading graphics plugins.");
	
	std::string graphicsPluginName = properties_->getStringValue("plugins.graphicsplugin");
	
	if (!graphicsPluginName.empty())
	{
		LOG_INFO(logger_, "Loading graphics plugin '" + graphicsPluginName + "'.");
		auto pluginBoostSharedPtr = boost::dll::import<ice_engine::IGraphicsPlugin>("./" + graphicsPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);
		
		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		graphicsPlugin_ = std::shared_ptr<ice_engine::IGraphicsPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IGraphicsPlugin*){});
	}
	
	LOG_INFO(logger_, "Finished loading graphics plugin.");
	
	LOG_INFO(logger_, "Loading module plugins.");
	
	std::vector<std::string> modulePluginNames;
	utilities::explode(properties_->getStringValue("plugins.moduleplugins"), ',', std::back_inserter(modulePluginNames));
	
	for (const auto& modulePluginName : modulePluginNames)
	{
		LOG_INFO(logger_, "Loading module plugin '" + modulePluginName + "'.");
		auto pluginBoostSharedPtr = boost::dll::import<ice_engine::IModulePlugin>("./" + modulePluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);
		
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
		LOG_INFO(logger_, "Loading scripting engine binding plugin '" + scriptingEngineBindingPluginName + "'.");
		auto pluginBoostSharedPtr = boost::dll::import<ice_engine::IScriptingEngineBindingPlugin>("./" + scriptingEngineBindingPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);

		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		auto pluginStdSharedPtr = std::shared_ptr<ice_engine::IScriptingEngineBindingPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](ice_engine::IScriptingEngineBindingPlugin*){});

		scriptingEngineBindingPlugins_.push_back( pluginStdSharedPtr );
	}

	LOG_INFO(logger_, "Finished loading scripting engine binding plugins.");

	LOG_INFO(logger_, "Finished loading plugins.");
}

PluginManager::~PluginManager()
{
}

const std::vector<std::shared_ptr<IGuiPlugin>>& PluginManager::getGuiPlugins() const
{
	return guiPlugins_;
}

std::shared_ptr<IGraphicsPlugin> PluginManager::getGraphicsPlugin() const
{
	return graphicsPlugin_;
}

const std::vector<std::shared_ptr<IModulePlugin>>& PluginManager::getModulePlugins() const
{
	return modulePlugins_;
}

const std::vector<std::shared_ptr<IScriptingEngineBindingPlugin>>& PluginManager::getScriptingEngineBindingPlugins() const
{
	return scriptingEngineBindingPlugins_;
}

}
