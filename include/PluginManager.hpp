#ifndef PLUGINMANAGER_H_
#define PLUGINMANAGER_H_

#include "IPluginManager.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{

class PluginManager : public IPluginManager
{
public:
		PluginManager(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	);
	virtual ~PluginManager();

	virtual const std::vector<std::shared_ptr<IGuiPlugin>>& getGuiPlugins() const override;
	virtual std::shared_ptr<IGraphicsPlugin> getGraphicsPlugin() const override;
	virtual const std::vector<std::shared_ptr<IModulePlugin>>& getModulePlugins() const override;
	virtual const std::vector<std::shared_ptr<IScriptingEngineBindingPlugin>>& getScriptingEngineBindingPlugins() const override;

private:
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	std::vector<std::shared_ptr<IGuiPlugin>> guiPlugins_;
	std::shared_ptr<IGraphicsPlugin> graphicsPlugin_;
	std::vector<std::shared_ptr<IModulePlugin>> modulePlugins_;
	std::vector<std::shared_ptr<IScriptingEngineBindingPlugin>> scriptingEngineBindingPlugins_;
};

}

#endif /* PLUGINMANAGER_H_ */
