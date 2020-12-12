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
	~PluginManager() override = default;

	const std::vector<std::shared_ptr<IGuiPlugin>>& getGuiPlugins() const override;
	std::shared_ptr<IGraphicsPlugin> getGraphicsPlugin() const override;
	std::shared_ptr<IAudioPlugin> getAudioPlugin() const override;
	std::shared_ptr<IPathfindingPlugin> getPathfindingPlugin() const override;
	std::shared_ptr<IPhysicsPlugin> getPhysicsPlugin() const override;
	std::shared_ptr<INetworkingPlugin> getNetworkingPlugin() const override;
	const std::vector<std::shared_ptr<IModulePlugin>>& getModulePlugins() const override;
	const std::vector<std::shared_ptr<IScriptingEngineBindingPlugin>>& scriptingEngineBindingPlugins() const override;

private:
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	
	std::vector<std::shared_ptr<IGuiPlugin>> guiPlugins_;
	std::shared_ptr<IGraphicsPlugin> graphicsPlugin_;
	std::shared_ptr<IAudioPlugin> audioPlugin_;
	std::shared_ptr<IPathfindingPlugin> pathfindingPlugin_;
	std::shared_ptr<IPhysicsPlugin> physicsPlugin_;
	std::shared_ptr<INetworkingPlugin> networkingPlugin_;
	std::vector<std::shared_ptr<IModulePlugin>> modulePlugins_;
	std::vector<std::shared_ptr<IScriptingEngineBindingPlugin>> scriptingEngineBindingPlugins_;
};

}

#endif /* PLUGINMANAGER_H_ */
