#include <string>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "ModelHandle.hpp"
#include "Scene.hpp"

#include "graphics/IGraphicsEngine.hpp"

#include "graphics/gui/IGui.hpp"

#include "graphics/model/Model.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "pathfinding/IPathfindingEngine.hpp"

#include "SceneBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "scripting/IScriptingEngine.hpp"

//#include "scripting/angel_script/scriptvector/scriptvector.hpp"

#include "GameEngine.hpp"

namespace ice_engine
{

namespace raybinding
{
void InitConstructor(const glm::vec3& from, const glm::vec3& to, void* memory) { new(memory) ray::Ray(from, to); }
}

SceneBindingDelegate::SceneBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine, audio::IAudioEngine* audioEngine, networking::INetworkingEngine* networkingEngine, physics::IPhysicsEngine* physicsEngine, pathfinding::IPathfindingEngine* pathfindingEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine),
	graphicsEngine_(graphicsEngine),
	audioEngine_(audioEngine),
	networkingEngine_(networkingEngine),
	physicsEngine_(physicsEngine),
	pathfindingEngine_(pathfindingEngine)
{
}

SceneBindingDelegate::~SceneBindingDelegate()
{
}

void SceneBindingDelegate::bind()
{
	scriptingEngine_->registerObjectType("Ray", sizeof(ray::Ray), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<ray::Ray>());
	scriptingEngine_->registerObjectBehaviour("Ray", asBEHAVE_CONSTRUCT, "void f(const vec3& in, const vec3& in)", asFUNCTION(raybinding::InitConstructor), asCALL_CDECL_OBJLAST);
	scriptingEngine_->registerObjectProperty("Ray", "vec3 from", asOFFSET(ray::Ray, from));
	scriptingEngine_->registerObjectProperty("Ray", "vec3 to", asOFFSET(ray::Ray, to));
	
	scriptingEngine_->registerObjectType("Raycast", sizeof(Raycast), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<Raycast>());
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"void setRay(const Ray& in)",
		asMETHODPR(Raycast, setRay, (const ray::Ray&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"const Ray& ray() const",
		asMETHODPR(Raycast, ray, () const, const ray::Ray&)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"void setHitPointWorld(const vec3& in)",
		asMETHODPR(Raycast, setHitPointWorld, (const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"const vec3& hitPointWorld() const",
		asMETHODPR(Raycast, hitPointWorld, () const, const glm::vec3&)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"void setHitNormalWorld(const vec3& in)",
		asMETHODPR(Raycast, setHitNormalWorld, (const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"const vec3& hitNormalWorld() const",
		asMETHODPR(Raycast, hitNormalWorld, () const, const glm::vec3&)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"void setEntity(const Entity& in)",
		asMETHODPR(Raycast, setEntity, (const entityx::Entity&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"const Entity& entity() const",
		asMETHODPR(Raycast, entity, () const, const entityx::Entity&)
	);
	//scriptingEngine_->registerObjectProperty("Raycast", "vec3 from", asOFFSET(ray::Ray, from));
	//scriptingEngine_->registerObjectProperty("Raycast", "vec3 to", asOFFSET(ray::Ray, to));

	scriptingEngine_->registerObjectType("SceneStatistics", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectProperty("SceneStatistics", "float physicsTime", asOFFSET(SceneStatistics, physicsTime));
	scriptingEngine_->registerObjectProperty("SceneStatistics", "float renderTime", asOFFSET(SceneStatistics, renderTime));
	
	// Scene
	scriptingEngine_->registerObjectType("Scene", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod("Scene", "string getName() const", asMETHODPR(Scene, getName, () const, std::string));
	scriptingEngine_->registerClassMethod("Scene", "bool visible() const", asMETHODPR(Scene, visible, () const, bool));
	scriptingEngine_->registerClassMethod(
		"Scene",
		"const SceneStatistics@ getSceneStatistics()",
		asMETHODPR(Scene, getSceneStatistics, () const, const SceneStatistics&)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void setDebugRendering(const bool)",
		asMETHODPR(Scene, setDebugRendering, (const bool), void )
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"CrowdHandle createCrowd(const NavigationMeshHandle& in)",
		asMETHODPR(Scene, createCrowd, (const pathfinding::NavigationMeshHandle&), pathfinding::CrowdHandle )
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"AgentHandle createAgent(const CrowdHandle& in, const vec3& in, const AgentParams& in)",
		asMETHODPR(Scene, createAgent, (const pathfinding::CrowdHandle&, const glm::vec3&, const pathfinding::AgentParams&), pathfinding::AgentHandle )
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void requestMoveTarget(const CrowdHandle& in, const AgentHandle& in, const vec3& in)",
		asMETHODPR(Scene, requestMoveTarget, (const pathfinding::CrowdHandle&, const pathfinding::AgentHandle&, const glm::vec3&), void )
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void requestMoveVelocity(const CrowdHandle& in, const AgentHandle& in, const vec3& in)",
		asMETHODPR(Scene, requestMoveVelocity, (const pathfinding::CrowdHandle&, const pathfinding::AgentHandle&, const glm::vec3&), void )
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"CollisionShapeHandle createStaticPlaneShape(const vec3& in, const float)",
		asMETHODPR(Scene, createStaticPlaneShape, (const glm::vec3&, const float32), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"CollisionShapeHandle createStaticBoxShape(const vec3& in)",
		asMETHODPR(Scene, createStaticBoxShape, (const glm::vec3&), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void destroyStaticShape(const CollisionShapeHandle& in)",
		asMETHODPR(Scene, destroyStaticShape, (const physics::CollisionShapeHandle&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void destroyAllStaticShapes()",
		asMETHODPR(Scene, destroyAllStaticShapes, (), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"RigidBodyObjectHandle createRigidBodyObject(const CollisionShapeHandle& in)",
		asMETHODPR(Scene, createRigidBodyObject, (const physics::CollisionShapeHandle&), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"RigidBodyObjectHandle createRigidBodyObject(const CollisionShapeHandle& in, const float, const float, const float)",
		asMETHODPR(Scene, createRigidBodyObject, (const physics::CollisionShapeHandle&, const float32, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"RigidBodyObjectHandle createRigidBodyObject(const CollisionShapeHandle& in, const vec3& in, const quat& in, const float, const float, const float)",
		asMETHODPR(Scene, createRigidBodyObject, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&, const float32, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"GhostObjectHandle createGhostObject(const CollisionShapeHandle& in)",
		asMETHODPR(Scene, createGhostObject, (const physics::CollisionShapeHandle&), physics::GhostObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"GhostObjectHandle createGhostObject(const CollisionShapeHandle& in, const vec3& in, const quat& in)",
		asMETHODPR(Scene, createGhostObject, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&), physics::GhostObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"RenderableHandle createRenderable(const ModelHandle& in, const string& in = string())",
		asMETHODPR(Scene, createRenderable, (const ModelHandle&, const std::string&), graphics::RenderableHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"RenderableHandle createRenderable(const MeshHandle& in, const TextureHandle& in, const string& in = string())",
		asMETHODPR(Scene, createRenderable, (const graphics::MeshHandle&, const graphics::TextureHandle&, const std::string&), graphics::RenderableHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"SoundSourceHandle play(const SoundHandle& in, const vec3& in)",
		asMETHODPR(Scene, play, (const audio::SoundHandle&, const glm::vec3&), audio::SoundSourceHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"PointLightHandle createPointLight(const vec3& in)",
		asMETHODPR(Scene, createPointLight, (const glm::vec3&), graphics::PointLightHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"ITerrain@ testCreateTerrain(HeightMap, SplatMap, DisplacementMap)",
		asMETHODPR(Scene, testCreateTerrain, (HeightMap, SplatMap, DisplacementMap), ITerrain*)
	);
	scriptingEngine_->registerClassMethod("Scene", "Entity createEntity()", asMETHODPR(Scene, createEntity, (), entityx::Entity));
	scriptingEngine_->registerClassMethod("Scene", "void destroy(const Entity& in)", asMETHODPR(Scene, destroy, (const entityx::Entity&), void));
	scriptingEngine_->registerClassMethod("Scene", "uint32 getNumEntities()", asMETHODPR(Scene, getNumEntities, () const, uint32));
	scriptingEngine_->registerClassMethod(
		"Scene",
		"Raycast raycast(const Ray& in)",
		asMETHODPR(Scene, raycast, (const ray::Ray&), Raycast)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void assign(const Entity& in, const GraphicsComponent& in)", 
		asMETHODPR(Scene, assign, (const entityx::Entity&, const ecs::GraphicsComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void assign(const Entity& in, const ScriptObjectComponent& in)",
		asMETHODPR(Scene, assign, (const entityx::Entity&, const ecs::ScriptObjectComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void assign(const Entity& in, const RigidBodyObjectComponent& in)", 
		asMETHODPR(Scene, assign, (const entityx::Entity&, const ecs::RigidBodyObjectComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void assign(const Entity& in, const GhostObjectComponent& in)", 
		asMETHODPR(Scene, assign, (const entityx::Entity&, const ecs::GhostObjectComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void assign(const Entity& in, const PathfindingAgentComponent& in)",
		asMETHODPR(Scene, assign, (const entityx::Entity&, const ecs::PathfindingAgentComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void assign(const Entity& in, const PointLightComponent& in)", 
		asMETHODPR(Scene, assign, (const entityx::Entity&, const ecs::PointLightComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void rotate(const Entity& in, const quat& in, const TransformSpace& in = TransformSpace::TS_LOCAL)",
		asMETHODPR(Scene, rotate, (const entityx::Entity&, const glm::quat&, const graphics::TransformSpace&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void rotate(const Entity& in, const float, const vec3& in, const TransformSpace& in = TransformSpace::TS_LOCAL)",
		asMETHODPR(Scene, rotate, (const entityx::Entity&, const float32, const glm::vec3&, const graphics::TransformSpace&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void rotation(const Entity& in, const quat& in)",
		asMETHODPR(Scene, rotation, (const entityx::Entity&, const glm::quat&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"quat rotation(const Entity& in) const",
		asMETHODPR(Scene, rotation, (const entityx::Entity&) const, glm::quat)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void scale(const Entity& in, const float)",
		asMETHODPR(Scene, scale, (const entityx::Entity&, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void scale(const Entity& in, const vec3& in)",
		asMETHODPR(Scene, scale, (const entityx::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"vec3 scale(const Entity& in) const",
		asMETHODPR(Scene, scale, (const entityx::Entity&) const, glm::vec3)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void scale(const Entity& in, const float, const float, const float)",
		asMETHODPR(Scene, scale, (const entityx::Entity&, const float32, const float32, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void translate(const Entity& in, const vec3& in)",
		asMETHODPR(Scene, translate, (const entityx::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void position(const Entity& in, const vec3& in)",
		asMETHODPR(Scene, position, (const entityx::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void position(const Entity& in, const float, const float, const float)",
		asMETHODPR(Scene, position, (const entityx::Entity&, const float32, const float32, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"vec3 position(const Entity& in) const",
		asMETHODPR(Scene, position, (const entityx::Entity&) const, glm::vec3)
	);
}
	
};
