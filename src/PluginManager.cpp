#include <iterator>
#include <vector>

#include <boost/dll/import.hpp>

#include "PluginManager.hpp"

#include "utilities/StringUtilities.hpp"

namespace hercules
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
	logger_->info("Loading plugins.");
	
	logger_->info("Loading gui plugins.");
	
	std::vector<std::string> guiPluginNames;
	utilities::explode(properties_->getStringValue("plugins.guiplugins"), ',', std::back_inserter(guiPluginNames));
	
	for (const auto& guiPluginName : guiPluginNames)
	{
		logger_->info("Loading gui plugin '" + guiPluginName + "'.");
		auto pluginBoostSharedPtr = boost::dll::import<hercules::IGuiPlugin>("./" + guiPluginName + "_plugin", "plugin", boost::dll::load_mode::append_decorations);
		
		// Convert from boost::shared_ptr<T> to std::shared_ptr<T>
		auto pluginStdSharedPtr = std::shared_ptr<hercules::IGuiPlugin>(pluginBoostSharedPtr.get(), [pluginBoostSharedPtr](hercules::IGuiPlugin*){});
		
		guiPlugins_.push_back( pluginStdSharedPtr );
	}
	
	logger_->info("Finished loading gui plugins.");
	
	logger_->info("Finished loading plugins.");
}

PluginManager::~PluginManager()
{
}

const std::vector<std::shared_ptr<IGuiPlugin>>& PluginManager::getGuiPlugins() const
{
	return guiPlugins_;
}

}
