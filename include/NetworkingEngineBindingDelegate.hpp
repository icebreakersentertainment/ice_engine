#ifndef NETWORKINGENGINEBINDINGDELEGATE_H_
#define NETWORKINGENGINEBINDINGDELEGATE_H_

namespace ice_engine
{

namespace scripting
{
class IScriptingEngine;
}
namespace networking
{
class INetworkingEngine;
}
namespace logger
{
class ILogger;
}

class GameEngine;

class NetworkingEngineBindingDelegate
{
public:
	NetworkingEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, networking::INetworkingEngine* networkingEngine);
	~NetworkingEngineBindingDelegate() = default;

	void bind();
	
private:
	logger::ILogger* logger_;
	GameEngine* gameEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	networking::INetworkingEngine* networkingEngine_;
};

}

#endif /* NETWORKINGENGINEBINDINGDELEGATE_H_ */

