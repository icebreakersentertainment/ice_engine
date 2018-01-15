#include "Platform.hpp"
#include "Types.hpp"

#include "networking/INetworkingEngine.hpp"

#include "NetworkingEngineBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "GameEngine.hpp"

namespace ice_engine
{

NetworkingEngineBindingDelegate::NetworkingEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, networking::INetworkingEngine* networkingEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine),
	networkingEngine_(networkingEngine)
{
}

void NetworkingEngineBindingDelegate::bind()
{
	// Enums available
	scriptingEngine_->registerEnum("NetworkingEventType");
	scriptingEngine_->registerEnumValue("NetworkingEventType", "UNKNOWN", networking::UNKNOWN);
	scriptingEngine_->registerEnumValue("NetworkingEventType", "SERVERDISCONNECT", networking::SERVERDISCONNECT);
	scriptingEngine_->registerEnumValue("NetworkingEventType", "CLIENTDISCONNECT", networking::CLIENTDISCONNECT);
	scriptingEngine_->registerEnumValue("NetworkingEventType", "SERVERCONNECT", networking::SERVERCONNECT);
	scriptingEngine_->registerEnumValue("NetworkingEventType", "SERVERCONNECTFAILED", networking::SERVERCONNECTFAILED);
	scriptingEngine_->registerEnumValue("NetworkingEventType", "CLIENTCONNECT", networking::CLIENTCONNECT);
	scriptingEngine_->registerEnumValue("NetworkingEventType", "SERVERMESSAGE", networking::SERVERMESSAGE);
	scriptingEngine_->registerEnumValue("NetworkingEventType", "CLIENTMESSAGE", networking::CLIENTMESSAGE);
	
	registerHandleBindings<networking::ServerHandle>(scriptingEngine_, "ServerHandle");
	registerHandleBindings<networking::ClientHandle>(scriptingEngine_, "ClientHandle");
	registerHandleBindings<networking::RemoteConnectionHandle>(scriptingEngine_, "RemoteConnectionHandle");

	scriptingEngine_->registerObjectType("ConnectEvent", sizeof(networking::ConnectEvent), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<networking::ConnectEvent>());
	scriptingEngine_->registerObjectProperty("ConnectEvent", "uint32 type", asOFFSET(networking::ConnectEvent, type));
	scriptingEngine_->registerObjectProperty("ConnectEvent", "uint32 timestamp", asOFFSET(networking::ConnectEvent, timestamp));
	scriptingEngine_->registerObjectProperty("ConnectEvent", "ServerHandle serverHandle", asOFFSET(networking::ConnectEvent, serverHandle));
	scriptingEngine_->registerObjectProperty("ConnectEvent", "ClientHandle clientHandle", asOFFSET(networking::ConnectEvent, clientHandle));
	scriptingEngine_->registerObjectProperty("ConnectEvent", "RemoteConnectionHandle remoteConnectionHandle", asOFFSET(networking::ConnectEvent, remoteConnectionHandle));
	scriptingEngine_->registerObjectType("DisconnectEvent", sizeof(networking::DisconnectEvent), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<networking::DisconnectEvent>());
	scriptingEngine_->registerObjectProperty("DisconnectEvent", "uint32 type", asOFFSET(networking::DisconnectEvent, type));
	scriptingEngine_->registerObjectProperty("DisconnectEvent", "uint32 timestamp", asOFFSET(networking::DisconnectEvent, timestamp));
	scriptingEngine_->registerObjectProperty("DisconnectEvent", "ServerHandle serverHandle", asOFFSET(networking::DisconnectEvent, serverHandle));
	scriptingEngine_->registerObjectProperty("DisconnectEvent", "ClientHandle clientHandle", asOFFSET(networking::DisconnectEvent, clientHandle));
	scriptingEngine_->registerObjectProperty("DisconnectEvent", "RemoteConnectionHandle remoteConnectionHandle", asOFFSET(networking::DisconnectEvent, remoteConnectionHandle));
	scriptingEngine_->registerObjectType("MessageEvent", sizeof(networking::MessageEvent), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<networking::MessageEvent>());
	scriptingEngine_->registerObjectProperty("MessageEvent", "uint32 type", asOFFSET(networking::MessageEvent, type));
	scriptingEngine_->registerObjectProperty("MessageEvent", "uint32 timestamp", asOFFSET(networking::MessageEvent, timestamp));
	scriptingEngine_->registerObjectProperty("MessageEvent", "ServerHandle serverHandle", asOFFSET(networking::MessageEvent, serverHandle));
	scriptingEngine_->registerObjectProperty("MessageEvent", "ClientHandle clientHandle", asOFFSET(networking::MessageEvent, clientHandle));
	scriptingEngine_->registerObjectProperty("MessageEvent", "RemoteConnectionHandle remoteConnectionHandle", asOFFSET(networking::MessageEvent, remoteConnectionHandle));
	scriptingEngine_->registerObjectProperty("MessageEvent", "vectorUInt8 message", asOFFSET(networking::MessageEvent, message));
	
	// INetworkingEngine
	scriptingEngine_->registerObjectType("INetworkingEngine", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("INetworkingEngine networking", networkingEngine_);
	scriptingEngine_->registerClassMethod(
		"INetworkingEngine",
		"ServerHandle createServer()",
		asMETHODPR(networking::INetworkingEngine, createServer, (), networking::ServerHandle)
	);
	scriptingEngine_->registerClassMethod(
		"INetworkingEngine",
		"ClientHandle createClient()",
		asMETHODPR(networking::INetworkingEngine, createClient, (), networking::ClientHandle)
	);
	scriptingEngine_->registerClassMethod(
		"INetworkingEngine",
		"void destroyServer(const ServerHandle& in)",
		asMETHODPR(networking::INetworkingEngine, destroyServer, (const networking::ServerHandle&), void)
	);
	scriptingEngine_->registerClassMethod(
		"INetworkingEngine",
		"void destroyClient(const ClientHandle& in)",
		asMETHODPR(networking::INetworkingEngine, destroyClient, (const networking::ClientHandle&), void)
	);
	scriptingEngine_->registerClassMethod(
		"INetworkingEngine",
		"void send(const ServerHandle& in, const vectorUInt8& in)",
		asMETHODPR(networking::INetworkingEngine, send, (const networking::ServerHandle&, const std::vector<uint8>&), void)
	);
	scriptingEngine_->registerClassMethod(
		"INetworkingEngine",
		"void send(const ServerHandle& in, const RemoteConnectionHandle& in, const vectorUInt8& in)",
		asMETHODPR(networking::INetworkingEngine, send, (const networking::ServerHandle&, const networking::RemoteConnectionHandle&, const std::vector<uint8>&), void)
	);
	scriptingEngine_->registerClassMethod(
		"INetworkingEngine",
		"void send(const ClientHandle& in, const vectorUInt8& in)",
		asMETHODPR(networking::INetworkingEngine, send, (const networking::ClientHandle&, const std::vector<uint8>&), void)
	);
	scriptingEngine_->registerClassMethod(
		"INetworkingEngine",
		"void processEvents()",
		asMETHODPR(networking::INetworkingEngine, processEvents, (), void)
	);
}
	
};
