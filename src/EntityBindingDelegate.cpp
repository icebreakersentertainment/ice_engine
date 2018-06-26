#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "ModelHandle.hpp"
#include "Scene.hpp"

#include "graphics/IGraphicsEngine.hpp"

#include "graphics/model/Model.hpp"

#include "EntityBindingDelegate.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "GameEngine.hpp"

namespace ice_engine
{	

EntityBindingDelegate::EntityBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine)
{
}

void EntityBindingDelegate::bind()
{
	scriptingEngine_->registerObjectType("Id", sizeof(entityx::Entity::Id), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entityx::Entity::Id>());
	scriptingEngine_->registerClassMethod("Id", "uint64 id() const", asMETHODPR(entityx::Entity::Id, id, () const, uint64));
	scriptingEngine_->registerClassMethod("Id", "uint32 index() const", asMETHODPR(entityx::Entity::Id, index, () const, uint32));
	scriptingEngine_->registerClassMethod("Id", "uint32 version() const", asMETHODPR(entityx::Entity::Id, version, () const, uint32));
	scriptingEngine_->registerClassMethod("Id", "bool opEquals(const Id& in) const", asMETHODPR(entityx::Entity::Id, operator==, (const entityx::Entity::Id&) const, bool));

	// Entity
	scriptingEngine_->registerObjectType("Entity", sizeof(entityx::Entity), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<entityx::Entity>());
	scriptingEngine_->registerObjectBehaviour("Entity", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<entityx::Entity>), asCALL_CDECL_OBJFIRST);
	//scriptingEngine_->registerObjectBehaviour("Entity", asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(InitConstructor<T>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Entity", asBEHAVE_CONSTRUCT, "void f(const Entity& in)", asFUNCTION(CopyConstructor<entityx::Entity>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Entity", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<entityx::Entity>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerClassMethod("Entity", "Entity& opAssign(const Entity& in)", asMETHODPR(entityx::Entity, operator=, (const entityx::Entity&), entityx::Entity&));
	scriptingEngine_->registerClassMethod("Entity", "Id id() const", asMETHODPR(entityx::Entity, id, () const, entityx::Entity::Id));
	scriptingEngine_->registerClassMethod("Entity", "bool opImplConv() const", asMETHODPR(entityx::Entity, operator bool, () const, bool ));
	scriptingEngine_->registerClassMethod("Entity", "bool opEquals(const Entity& in) const", asMETHODPR(entityx::Entity, operator==, (const entityx::Entity&) const, bool));

	scriptingEngine_->registerObjectType("RigidBodyObjectComponent", sizeof(ecs::RigidBodyObjectComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<ecs::RigidBodyObjectComponent>());
	scriptingEngine_->registerObjectProperty("RigidBodyObjectComponent", "RigidBodyObjectHandle rigidBodyObjectHandle", asOFFSET(ecs::RigidBodyObjectComponent, rigidBodyObjectHandle));
	scriptingEngine_->registerObjectType("GhostObjectComponent", sizeof(ecs::GhostObjectComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<ecs::GhostObjectComponent>());
	scriptingEngine_->registerObjectProperty("GhostObjectComponent", "GhostObjectHandle ghostObjectHandle", asOFFSET(ecs::GhostObjectComponent, ghostObjectHandle));
	scriptingEngine_->registerObjectType("PositionOrientationComponent", sizeof(ecs::PositionOrientationComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<ecs::PositionOrientationComponent>());
	scriptingEngine_->registerObjectProperty("PositionOrientationComponent", "vec3 position", asOFFSET(ecs::PositionOrientationComponent, position));
	scriptingEngine_->registerObjectProperty("PositionOrientationComponent", "quat orientation", asOFFSET(ecs::PositionOrientationComponent, orientation));
	scriptingEngine_->registerObjectType("PointLightComponent", sizeof(ecs::PointLightComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<ecs::PointLightComponent>());
	scriptingEngine_->registerObjectProperty("PointLightComponent", "vec3 position", asOFFSET(ecs::PointLightComponent, position));
	scriptingEngine_->registerObjectProperty("PointLightComponent", "PointLightHandle pointLightHandle", asOFFSET(ecs::PointLightComponent, pointLightHandle));
	
	registerComponent<ecs::PathfindingAgentComponent, pathfinding::CrowdHandle, pathfinding::AgentHandle>(
		scriptingEngine_,
		"PathfindingAgentComponent",
		{
			{"CrowdHandle crowdHandle", asOFFSET(ecs::PathfindingAgentComponent, crowdHandle)},
			{"AgentHandle agentHandle", asOFFSET(ecs::PathfindingAgentComponent, agentHandle)}
		},
		"CrowdHandle crowdHandle, AgentHandle agentHandle"
	);
	registerComponent<ecs::GraphicsComponent, ModelHandle, glm::vec3, graphics::RenderableHandle>(
		scriptingEngine_,
		"GraphicsComponent",
		{
			{"ModelHandle modelHandle", asOFFSET(ecs::GraphicsComponent, modelHandle)},
			{"vec3 scale", asOFFSET(ecs::GraphicsComponent, scale)},
			{"RenderableHandle renderableHandle", asOFFSET(ecs::GraphicsComponent, renderableHandle)}
		}
	);
	registerEntityComponentAssignMethod<ecs::GraphicsComponent, ModelHandle>(scriptingEngine_, "GraphicsComponent", "ModelHandle modelHandle");
	registerComponent<ecs::ScriptObjectComponent, scripting::ScriptObjectHandle>(
		scriptingEngine_,
		"ScriptObjectComponent",
		{
			{"ScriptObjectHandle scriptObjectHandle", asOFFSET(ecs::ScriptObjectComponent, scriptObjectHandle)}
		}
	);

	scriptingEngine_->registerObjectType("ComponentHandlePositionOrientationComponent", sizeof(entityx::ComponentHandle<ecs::PositionOrientationComponent>), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<entityx::ComponentHandle<ecs::PositionOrientationComponent>>());
	scriptingEngine_->registerObjectBehaviour("ComponentHandlePositionOrientationComponent", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<entityx::ComponentHandle<ecs::PositionOrientationComponent>>), asCALL_CDECL_OBJFIRST);
	//scriptingEngine_->registerObjectBehaviour("ComponentHandlePositionOrientationComponent", asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(InitConstructor<T>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("ComponentHandlePositionOrientationComponent", asBEHAVE_CONSTRUCT, "void f(const ComponentHandlePositionOrientationComponent& in)", asFUNCTION(CopyConstructor<entityx::ComponentHandle<ecs::PositionOrientationComponent>>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("ComponentHandlePositionOrientationComponent", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<entityx::ComponentHandle<ecs::PositionOrientationComponent>>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerClassMethod("ComponentHandlePositionOrientationComponent", "ComponentHandlePositionOrientationComponent& opAssign(const ComponentHandlePositionOrientationComponent& in)", asMETHODPR(entityx::ComponentHandle<ecs::PositionOrientationComponent>, operator=, (const entityx::ComponentHandle<ecs::PositionOrientationComponent>&), entityx::ComponentHandle<ecs::PositionOrientationComponent>&));
	scriptingEngine_->registerClassMethod("ComponentHandlePositionOrientationComponent", "bool opEquals(const ComponentHandlePositionOrientationComponent& in) const", asMETHODPR(entityx::ComponentHandle<ecs::PositionOrientationComponent>, operator==, (const entityx::ComponentHandle<ecs::PositionOrientationComponent>&) const, bool));
	scriptingEngine_->registerClassMethod("ComponentHandlePositionOrientationComponent", "bool opImplConv() const", asMETHODPR(entityx::ComponentHandle<ecs::PositionOrientationComponent>, operator bool, () const, bool )); 
	/*
	scriptingEngine_->registerClassMethod(
		"ComponentHandlePositionOrientationComponent",
		"PositionOrientationComponent@ get()",
		//asFUNCTION((component<ecs::PositionOrientationComponent>)),
		//asCALL_CDECL_OBJFIRST
		asMETHODPR(entityx::ComponentHandle<ecs::PositionOrientationComponent>, get, (), ecs::PositionOrientationComponent*)
	);*/
	
	scriptingEngine_->registerObjectType("ComponentHandleRigidBodyObjectComponent", sizeof(entityx::ComponentHandle<ecs::RigidBodyObjectComponent>), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<entityx::ComponentHandle<ecs::RigidBodyObjectComponent>>());
	scriptingEngine_->registerObjectBehaviour("ComponentHandleRigidBodyObjectComponent", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<entityx::ComponentHandle<ecs::RigidBodyObjectComponent>>), asCALL_CDECL_OBJFIRST);
	//scriptingEngine_->registerObjectBehaviour("ComponentHandleRigidBodyObjectComponent", asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(InitConstructor<T>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("ComponentHandleRigidBodyObjectComponent", asBEHAVE_CONSTRUCT, "void f(const ComponentHandleRigidBodyObjectComponent& in)", asFUNCTION(CopyConstructor<entityx::ComponentHandle<ecs::RigidBodyObjectComponent>>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("ComponentHandleRigidBodyObjectComponent", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<entityx::ComponentHandle<ecs::RigidBodyObjectComponent>>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerClassMethod("ComponentHandleRigidBodyObjectComponent", "ComponentHandleRigidBodyObjectComponent& opAssign(const ComponentHandleRigidBodyObjectComponent& in)", asMETHODPR(entityx::ComponentHandle<ecs::RigidBodyObjectComponent>, operator=, (const entityx::ComponentHandle<ecs::RigidBodyObjectComponent>&), entityx::ComponentHandle<ecs::RigidBodyObjectComponent>&));
	scriptingEngine_->registerClassMethod("ComponentHandleRigidBodyObjectComponent", "bool opEquals(const ComponentHandleRigidBodyObjectComponent& in) const", asMETHODPR(entityx::ComponentHandle<ecs::RigidBodyObjectComponent>, operator==, (const entityx::ComponentHandle<ecs::RigidBodyObjectComponent>&) const, bool));
	scriptingEngine_->registerClassMethod("ComponentHandleRigidBodyObjectComponent", "bool opImplConv() const", asMETHODPR(entityx::ComponentHandle<ecs::RigidBodyObjectComponent>, operator bool, () const, bool )); 

	//registerHandleBindings<ecs::Entity>(scriptingEngine_, "Entity");
	
	scriptingEngine_->registerObjectMethod(
		"Entity",
		"ComponentHandlePositionOrientationComponent assignPositionOrientationComponent(vec3, quat)",
		asFUNCTION((assign<ecs::PositionOrientationComponent, glm::vec3, glm::quat>)),
		asCALL_CDECL_OBJFIRST
		//asMETHODPR(entityx::Entity, assign<ecs::PositionOrientationComponent>, (glm::vec3, glm::quat), entityx::ComponentHandle<ecs::PositionOrientationComponent>)
	);
	scriptingEngine_->registerClassMethod(
		"Entity",
		"bool has_componentPositionOrientationComponent() const",
		//asFUNCTION((component<PositionOrientationComponent>)),
		//asCALL_CDECL_OBJFIRST
		asMETHODPR(entityx::Entity, has_component<ecs::PositionOrientationComponent>, () const, bool)
	);
	scriptingEngine_->registerClassMethod(
		"Entity",
		"ComponentHandlePositionOrientationComponent componentPositionOrientationComponent()",
		//asFUNCTION((component<PositionOrientationComponent>)),
		//asCALL_CDECL_OBJFIRST
		asMETHODPR(entityx::Entity, component<ecs::PositionOrientationComponent>, (), entityx::ComponentHandle<ecs::PositionOrientationComponent>)
	);
	
	/*
	scriptingEngine_->registerObjectMethod(
		"Entity",
		"ComponentHandleRigidBodyObjectComponent assignRigidBodyObjectComponent(vec3, quat)",
		asFUNCTION((assign<ecs::RigidBodyObjectComponent, glm::vec3, glm::quat>)),
		asCALL_CDECL_OBJFIRST
		//asMETHODPR(entityx::Entity, assign<ecs::RigidBodyObjectComponent>, (glm::vec3, glm::quat), entityx::ComponentHandle<ecs::RigidBodyObjectComponent>)
	);*/
	scriptingEngine_->registerClassMethod(
		"Entity",
		"bool has_componentRigidBodyObjectComponent() const",
		//asFUNCTION((component<RigidBodyObjectComponent>)),
		//asCALL_CDECL_OBJFIRST
		asMETHODPR(entityx::Entity, has_component<ecs::RigidBodyObjectComponent>, () const, bool)
	);
	scriptingEngine_->registerClassMethod(
		"Entity",
		"ComponentHandleRigidBodyObjectComponent componentRigidBodyObjectComponent()",
		//asFUNCTION((component<RigidBodyObjectComponent>)),
		//asCALL_CDECL_OBJFIRST
		asMETHODPR(entityx::Entity, component<ecs::RigidBodyObjectComponent>, (), entityx::ComponentHandle<ecs::RigidBodyObjectComponent>)
	);
}
	
};
