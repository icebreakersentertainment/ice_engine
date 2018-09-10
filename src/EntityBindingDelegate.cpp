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
	scriptingEngine_->registerObjectType("Entity", sizeof(ecs::Entity), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<ecs::Entity>());
	scriptingEngine_->registerObjectBehaviour("Entity", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<ecs::Entity>), asCALL_CDECL_OBJFIRST);
	//scriptingEngine_->registerObjectBehaviour("Entity", asBEHAVE_CONSTRUCT, "void f(const uint64)", asFUNCTION(InitConstructor<T>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Entity", asBEHAVE_CONSTRUCT, "void f(const Entity& in)", asFUNCTION(CopyConstructor<ecs::Entity>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerObjectBehaviour("Entity", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<ecs::Entity>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerClassMethod("Entity", "Entity& opAssign(const Entity& in)", asMETHODPR(ecs::Entity, operator=, (const ecs::Entity&), ecs::Entity&));
	scriptingEngine_->registerClassMethod("Entity", "Id id() const", asMETHODPR(ecs::Entity, id, () const, entityx::Entity::Id));
	scriptingEngine_->registerClassMethod("Entity", "void destroy()", asMETHOD(ecs::Entity, destroy));
	scriptingEngine_->registerClassMethod("Entity", "bool opImplConv() const", asMETHODPR(ecs::Entity, operator bool, () const, bool ));
	scriptingEngine_->registerClassMethod("Entity", "bool opEquals(const Entity& in) const", asMETHODPR(ecs::Entity, operator==, (const ecs::Entity&) const, bool));
	scriptingEngine_->registerClassMethod("Entity", "Scene@ scene() const", asMETHOD(ecs::Entity, scene));
	
	registerComponent<ecs::PositionComponent, glm::vec3>(
		scriptingEngine_,
		"PositionComponent",
		{
			{"vec3 position", asOFFSET(ecs::PositionComponent, position)}
		},
		"vec3"
	);

	registerComponent<ecs::OrientationComponent, glm::quat>(
		scriptingEngine_,
		"OrientationComponent",
		{
			{"quat orientation", asOFFSET(ecs::OrientationComponent, orientation)}
		},
		"quat"
	);

	registerComponent<ecs::GhostObjectComponent, physics::CollisionShapeHandle, physics::GhostObjectHandle>(
		scriptingEngine_,
		"GhostObjectComponent",
		{
			{"CollisionShapeHandle collisionShapeHandle", asOFFSET(ecs::GhostObjectComponent, collisionShapeHandle)},
			{"GhostObjectHandle ghostObjectHandle", asOFFSET(ecs::GhostObjectComponent, ghostObjectHandle)}
		},
		"CollisionShapeHandle, GhostObjectHandle"
	);
	registerEntityComponentAssignMethodNoForward<ecs::GhostObjectComponent, physics::CollisionShapeHandle>(scriptingEngine_, "GhostObjectComponent", "CollisionShapeHandle");

	registerComponent<ecs::PointLightComponent, graphics::PointLightHandle>(
		scriptingEngine_,
		"PointLightComponent",
		{
			{"PointLightHandle pointLightHandle", asOFFSET(ecs::PointLightComponent, pointLightHandle)}
		},
		"PointLightHandle"
	);

	registerComponent<ecs::RigidBodyObjectComponent, physics::CollisionShapeHandle, float32, float32, float32, physics::RigidBodyObjectHandle>(
		scriptingEngine_,
		"RigidBodyObjectComponent",
		{
			{"CollisionShapeHandle collisionShapeHandle", asOFFSET(ecs::RigidBodyObjectComponent, collisionShapeHandle)},
			{"float mass", asOFFSET(ecs::RigidBodyObjectComponent, mass)},
			{"float friction", asOFFSET(ecs::RigidBodyObjectComponent, friction)},
			{"float restitution", asOFFSET(ecs::RigidBodyObjectComponent, restitution)},
			{"RigidBodyObjectHandle rigidBodyObjectHandle", asOFFSET(ecs::RigidBodyObjectComponent, rigidBodyObjectHandle)}
		},
		"CollisionShapeHandle, float, float, float, RigidBodyObjectHandle"
	);
	registerEntityComponentAssignMethodNoForward<ecs::RigidBodyObjectComponent, physics::CollisionShapeHandle>(scriptingEngine_, "RigidBodyObjectComponent", "CollisionShapeHandle");
	registerEntityComponentAssignMethodNoForward<ecs::RigidBodyObjectComponent, physics::CollisionShapeHandle, float32, float32, float32>(scriptingEngine_, "RigidBodyObjectComponent", "CollisionShapeHandle, float = 1.0f, float = 1.0f, float = 1.0f");

	registerComponent<ecs::PathfindingCrowdComponent, pathfinding::NavigationMeshHandle, pathfinding::CrowdHandle>(
				scriptingEngine_,
				"PathfindingCrowdComponent",
				{
					{"NavigationMeshHandle navigationMeshHandle", asOFFSET(ecs::PathfindingCrowdComponent, navigationMeshHandle)},
					{"CrowdHandle crowdHandle", asOFFSET(ecs::PathfindingCrowdComponent, crowdHandle)}

				},
				"NavigationMeshHandle, CrowdHandle"
			);
		registerEntityComponentAssignMethodNoForward<ecs::PathfindingCrowdComponent, pathfinding::NavigationMeshHandle>(scriptingEngine_, "PathfindingCrowdComponent", "NavigationMeshHandle");

	registerComponent<ecs::PathfindingAgentComponent, pathfinding::CrowdHandle, pathfinding::AgentHandle, pathfinding::AgentParams, glm::vec3>(
		scriptingEngine_,
		"PathfindingAgentComponent",
		{
			{"CrowdHandle crowdHandle", asOFFSET(ecs::PathfindingAgentComponent, crowdHandle)},
			{"AgentHandle agentHandle", asOFFSET(ecs::PathfindingAgentComponent, agentHandle)},
			{"AgentParams agentParams", asOFFSET(ecs::PathfindingAgentComponent, agentParams)},
			{"AgentState  agentState", asOFFSET(ecs::PathfindingAgentComponent, agentState)},
			{"MovementRequestState  movementRequestState", asOFFSET(ecs::PathfindingAgentComponent, movementRequestState)},
			{"vec3 target", asOFFSET(ecs::PathfindingAgentComponent, target)},
		},
		"CrowdHandle, AgentHandle, AgentParams = AgentParams(), vec3 = vec3()"
	);
	registerEntityComponentAssignMethodNoForward<ecs::PathfindingAgentComponent, pathfinding::CrowdHandle, pathfinding::AgentParams, glm::vec3>(scriptingEngine_, "PathfindingAgentComponent", "CrowdHandle, AgentParams, vec3");

	registerComponent<ecs::GraphicsComponent, ModelHandle, glm::vec3, graphics::RenderableHandle>(
		scriptingEngine_,
		"GraphicsComponent",
		{
			{"ModelHandle modelHandle", asOFFSET(ecs::GraphicsComponent, modelHandle)},
			{"vec3 scale", asOFFSET(ecs::GraphicsComponent, scale)},
			{"RenderableHandle renderableHandle", asOFFSET(ecs::GraphicsComponent, renderableHandle)}
		}
	);
	registerEntityComponentAssignMethodNoForward<ecs::GraphicsComponent, ModelHandle>(scriptingEngine_, "GraphicsComponent", "ModelHandle");
	registerEntityComponentAssignMethodNoForward<ecs::GraphicsComponent, ModelHandle, glm::vec3>(scriptingEngine_, "GraphicsComponent", "ModelHandle, vec3");

	registerComponent<ecs::GraphicsTerrainComponent, graphics::TerrainHandle, graphics::TerrainRenderableHandle>(
		scriptingEngine_,
		"GraphicsTerrainComponent",
		{
			{"TerrainHandle terrainHandle", asOFFSET(ecs::GraphicsTerrainComponent, terrainHandle)},
			{"TerrainRenderableHandle terrainRenderableHandle", asOFFSET(ecs::GraphicsTerrainComponent, terrainRenderableHandle)}
		},
		"TerrainHandle, TerrainRenderableHandle"
	);
	registerEntityComponentAssignMethodNoForward<ecs::GraphicsTerrainComponent, graphics::TerrainHandle>(scriptingEngine_, "GraphicsTerrainComponent", "TerrainHandle");

	registerComponent<ecs::ScriptObjectComponent, scripting::ScriptObjectHandle>(
		scriptingEngine_,
		"ScriptObjectComponent",
		{
			{"ScriptObjectHandle scriptObjectHandle", asOFFSET(ecs::ScriptObjectComponent, scriptObjectHandle)}
		},
		"ScriptObjectHandle"
	);
//	registerEntityComponentAssignMethodNoForward<ecs::ScriptObjectComponent, scripting::ScriptObjectHandle>(scriptingEngine_, "ScriptObjectComponent", "ScriptObjectHandle");
	registerEntityComponentAssignMethodNoForward<ecs::ScriptObjectComponent, void*>(scriptingEngine_, "ScriptObjectComponent", "IScriptObject@");

	scriptingEngine_->registerEnum("DirtyFlags");
	scriptingEngine_->registerEnumValue("DirtyFlags", "DIRTY_SOURCE_SCRIPT", ecs::DirtyFlags::DIRTY_SOURCE_SCRIPT);
	scriptingEngine_->registerEnumValue("DirtyFlags", "DIRTY_POSITION", ecs::DirtyFlags::DIRTY_POSITION);
	registerComponent<ecs::DirtyComponent, uint16>(
		scriptingEngine_,
		"DirtyComponent",
		{
			{"uint16 dirty", asOFFSET(ecs::DirtyComponent, dirty)}
		},
		"uint16"
	);

	registerComponent<ecs::PersistableComponent>(
		scriptingEngine_,
		"PersistableComponent",
		{}
	);

//	enum DirtyFlags : uint16
//	{
//		DIRTY_SOURCE_SCRIPT				= 1 << 0,
//		DIRTY_SOURCE_PHYSICS			= 1 << 1,
//		DIRTY_SOURCE_PATHFINDING		= 1 << 2,
//
//		DIRTY_ALL						= 1 << 3,
//		DIRTY_POSITION					= 1 << 4,
//		DIRTY_ORIENTATION				= 1 << 5,
//		DIRTY_RIGID_BODY_OBJECT			= 1 << 6,
//		DIRTY_GHOST_OBJECT				= 1 << 7,
//		DIRTY_PATHFINDING_AGENT			= 1 << 8,
//	};
}
	
};
