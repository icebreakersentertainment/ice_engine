#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

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
	scriptingEngine_->registerObjectType("Id", sizeof(entityx::Entity::Id), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<entityx::Entity::Id>());
    scriptingEngine_->debugger()->registerToStringCallback("Id", scriptingEngineDebuggerToStringCallback<entityx::Entity::Id>());
//	scriptingEngine_->registerObjectBehaviour("Id", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(DefaultConstructor<entityx::Entity::Id>), asCALL_CDECL_OBJFIRST);
//	scriptingEngine_->registerObjectBehaviour("Id", asBEHAVE_CONSTRUCT, "void f(const Id& in)", asFUNCTION(CopyConstructor<entityx::Entity::Id>), asCALL_CDECL_OBJFIRST);
//	scriptingEngine_->registerObjectBehaviour("Id", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(DefaultDestructor<entityx::Entity::Id>), asCALL_CDECL_OBJFIRST);
	scriptingEngine_->registerClassMethod("Id", "uint64 id() const", asMETHODPR(entityx::Entity::Id, id, () const, uint64));
	scriptingEngine_->registerClassMethod("Id", "uint32 index() const", asMETHODPR(entityx::Entity::Id, index, () const, uint32));
	scriptingEngine_->registerClassMethod("Id", "uint32 version() const", asMETHODPR(entityx::Entity::Id, version, () const, uint32));
	scriptingEngine_->registerClassMethod("Id", "bool opEquals(const Id& in) const", asMETHODPR(entityx::Entity::Id, operator==, (const entityx::Entity::Id&) const, bool));

	// Entity
	scriptingEngine_->registerObjectType("Entity", sizeof(ecs::Entity), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<ecs::Entity>());
	scriptingEngine_->debugger()->registerToStringCallback("Entity", scriptingEngineDebuggerToStringCallback<ecs::Entity>());
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

	registerVectorBindings<ecs::Entity>(scriptingEngine_, "vectorEntity", "Entity");

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

	registerComponent<ecs::PathfindingCrowdComponent, pathfinding::NavigationMeshHandle, pathfinding::CrowdConfig, pathfinding::CrowdHandle>(
				scriptingEngine_,
				"PathfindingCrowdComponent",
				{
					{"NavigationMeshHandle navigationMeshHandle", asOFFSET(ecs::PathfindingCrowdComponent, navigationMeshHandle)},
					{"CrowdConfig crowdConfig", asOFFSET(ecs::PathfindingCrowdComponent, crowdConfig)},
					{"CrowdHandle crowdHandle", asOFFSET(ecs::PathfindingCrowdComponent, crowdHandle)}

				},
				"NavigationMeshHandle, CrowdConfig, CrowdHandle"
			);
		registerEntityComponentAssignMethodNoForward<ecs::PathfindingCrowdComponent, pathfinding::NavigationMeshHandle>(scriptingEngine_, "PathfindingCrowdComponent", "NavigationMeshHandle");
		registerEntityComponentAssignMethodNoForward<ecs::PathfindingCrowdComponent, pathfinding::NavigationMeshHandle, pathfinding::CrowdConfig>(scriptingEngine_, "PathfindingCrowdComponent", "NavigationMeshHandle, CrowdConfig");

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
//	registerEntityComponentAssignMethodNoForward2<ecs::PathfindingAgentComponent, pathfinding::CrowdHandle, pathfinding::AgentParams, glm::vec3>(scriptingEngine_, "PathfindingAgentComponent", "CrowdHandle, AgentParams, vec3");
	registerEntityComponentAssignMethodNoForward<ecs::PathfindingAgentComponent, const pathfinding::CrowdHandle&, const pathfinding::AgentParams&, const glm::vec3&>(scriptingEngine_, "PathfindingAgentComponent", "const CrowdHandle& in, const AgentParams& in, const vec3& in");

	registerComponent<ecs::PathfindingObstacleComponent, pathfinding::PolygonMeshHandle, float32, float32, pathfinding::ObstacleHandle>(
		scriptingEngine_,
		"PathfindingObstacleComponent",
		{
			{"PolygonMeshHandle polygonMeshHandle", asOFFSET(ecs::PathfindingObstacleComponent, polygonMeshHandle)},
			{"float radius", asOFFSET(ecs::PathfindingObstacleComponent, radius)},
			{"float height", asOFFSET(ecs::PathfindingObstacleComponent, height)},
			{"ObstacleHandle obstacleHandle", asOFFSET(ecs::PathfindingObstacleComponent, obstacleHandle)},
		},
		"PolygonMeshHandle, float = 0.0f, float = 0.0f, ObstacleHandle = ObstacleHandle()"
	);

	registerComponent<ecs::GraphicsComponent, graphics::MeshHandle, graphics::TextureHandle, glm::vec3, graphics::RenderableHandle>(
		scriptingEngine_,
		"GraphicsComponent",
		{
			{"MeshHandle meshHandle", asOFFSET(ecs::GraphicsComponent, meshHandle)},
			{"TextureHandle textureHandle", asOFFSET(ecs::GraphicsComponent, textureHandle)},
			{"vec3 scale", asOFFSET(ecs::GraphicsComponent, scale)},
			{"RenderableHandle renderableHandle", asOFFSET(ecs::GraphicsComponent, renderableHandle)}
		}
	);
	registerEntityComponentAssignMethodNoForward<ecs::GraphicsComponent, graphics::MeshHandle>(scriptingEngine_, "GraphicsComponent", "MeshHandle");
	registerEntityComponentAssignMethodNoForward<ecs::GraphicsComponent, graphics::MeshHandle, graphics::TextureHandle>(scriptingEngine_, "GraphicsComponent", "MeshHandle, TextureHandle");
	registerEntityComponentAssignMethodNoForward<ecs::GraphicsComponent, graphics::MeshHandle, graphics::TextureHandle, glm::vec3>(scriptingEngine_, "GraphicsComponent", "MeshHandle, TextureHandle, vec3");

	registerComponent<ecs::AnimationComponent, AnimationHandle>(
		scriptingEngine_,
		"AnimationComponent",
		{
			{"AnimationHandle animationHandle", asOFFSET(ecs::AnimationComponent, animationHandle)},
			{"BonesHandle bonesHandle", asOFFSET(ecs::AnimationComponent, bonesHandle)},
			{"chrono::durationFloat runningTime", asOFFSET(ecs::AnimationComponent, runningTime)},
			{"float speed", asOFFSET(ecs::AnimationComponent, speed)},
			{"uint32 startFrame", asOFFSET(ecs::AnimationComponent, startFrame)},
			{"uint32 endFrame", asOFFSET(ecs::AnimationComponent, endFrame)},
			{"vectorMat4 transformations", asOFFSET(ecs::AnimationComponent, transformations)}
		},
		"AnimationHandle"
	);
	registerEntityComponentAssignMethodNoForward<ecs::AnimationComponent, AnimationHandle, uint32, uint32>(scriptingEngine_, "AnimationComponent", "AnimationHandle, uint32, uint32");

	registerComponent<ecs::SkeletonComponent, SkeletonHandle>(
		scriptingEngine_,
		"SkeletonComponent",
		{
			{"SkeletonHandle skeletonHandle", asOFFSET(ecs::SkeletonComponent, skeletonHandle)}
		}
	);

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

	registerComponent<ecs::GraphicsSkyboxComponent, graphics::SkyboxHandle, graphics::SkyboxRenderableHandle>(
		scriptingEngine_,
		"GraphicsSkyboxComponent",
		{
			{"SkyboxHandle skyboxHandle", asOFFSET(ecs::GraphicsSkyboxComponent, skyboxHandle)},
			{"SkyboxRenderableHandle skyboxRenderableHandle", asOFFSET(ecs::GraphicsSkyboxComponent, skyboxRenderableHandle)}
		},
		"SkyboxHandle, SkyboxRenderableHandle"
	);
	registerEntityComponentAssignMethodNoForward<ecs::GraphicsSkyboxComponent, graphics::SkyboxHandle>(scriptingEngine_, "GraphicsSkyboxComponent", "SkyboxHandle");

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
	scriptingEngine_->registerEnumValue("DirtyFlags", "DIRTY_ORIENTATION", ecs::DirtyFlags::DIRTY_ORIENTATION);
	scriptingEngine_->registerEnumValue("DirtyFlags", "DIRTY_PATHFINDING_AGENT", ecs::DirtyFlags::DIRTY_PATHFINDING_AGENT);
	registerComponent<ecs::DirtyComponent, uint16>(
		scriptingEngine_,
		"DirtyComponent",
		{
			{"uint16 dirty", asOFFSET(ecs::DirtyComponent, dirty)}
		},
		"uint16"
	);

	registerComponent<ecs::ParentComponent>(
			scriptingEngine_,
			"ParentComponent",
			{
				{"Entity entity", asOFFSET(ecs::ParentComponent, entity)}
			}
		);
	// for some reason on Linux using the no forward version causes the entity to be invalid
	registerEntityComponentAssignMethod<ecs::ParentComponent, ecs::Entity>(scriptingEngine_, "ParentComponent", "Entity");

	registerComponent<ecs::ChildrenComponent, std::vector<ecs::Entity>>(
			scriptingEngine_,
			"ChildrenComponent",
			{
				{"vectorEntity children", asOFFSET(ecs::ChildrenComponent, children)}
			},
			"vectorEntity"
		);

	registerComponent<ecs::ParentBoneAttachmentComponent, std::string, glm::ivec4, glm::vec4>(
			scriptingEngine_,
			"ParentBoneAttachmentComponent",
			{
				{"string boneName", asOFFSET(ecs::ParentBoneAttachmentComponent, boneName)},
				{"ivec4 boneIds", asOFFSET(ecs::ParentBoneAttachmentComponent, boneIds)},
				{"vec4 boneWeights", asOFFSET(ecs::ParentBoneAttachmentComponent, boneWeights)}
			},
			"string, ivec4, vec4"
		);
	registerEntityComponentAssignMethodNoForward<ecs::ParentBoneAttachmentComponent, std::string>(scriptingEngine_, "ParentBoneAttachmentComponent", "string");

    registerComponent<ecs::PropertiesComponent, std::unordered_map<std::string, std::string>>(
            scriptingEngine_,
            "PropertiesComponent",
            {
                    {"unordered_mapStringString properties", asOFFSET(ecs::PropertiesComponent, properties)}
            },
            "unordered_mapStringString"
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
