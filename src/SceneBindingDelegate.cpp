#include <string>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

//#include <boost/variant.hpp>

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
		asMETHODPR(Raycast, setEntity, (const ecs::Entity&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"Entity entity() const",
		asMETHODPR(Raycast, entity, () const, ecs::Entity)
	);
	//scriptingEngine_->registerObjectProperty("Raycast", "vec3 from", asOFFSET(ray::Ray, from));
	//scriptingEngine_->registerObjectProperty("Raycast", "vec3 to", asOFFSET(ray::Ray, to));

//	registerVariantBindings<physics::RigidBodyObjectHandle, physics::GhostObjectHandle>(
//		scriptingEngine_,
//		"variantRigidBodyObjectHandleGhostObjectHandle",
//		{"RigidBodyObjectHandle", "GhostObjectHandle"}
//	);

//	registerVectorBindings<boost::variant<physics::RigidBodyObjectHandle, physics::GhostObjectHandle>>(scriptingEngine_, "vectorVariantRigidBodyObjectHandleGhostObjectHandle", "variantRigidBodyObjectHandleGhostObjectHandle");

	scriptingEngine_->registerObjectType("SceneStatistics", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectProperty("SceneStatistics", "float physicsTime", asOFFSET(SceneStatistics, physicsTime));
	scriptingEngine_->registerObjectProperty("SceneStatistics", "float renderTime", asOFFSET(SceneStatistics, renderTime));
	
	scriptingEngine_->registerFunctionDefinition("void PreSerializeCallback(Scene@)");
	scriptingEngine_->registerFunctionDefinition("void PostSerializeCallback(Scene@)");
	scriptingEngine_->registerFunctionDefinition("void PreDeserializeCallback(Scene@)");
	scriptingEngine_->registerFunctionDefinition("void PostDeserializeCallback(Scene@)");

	scriptingEngine_->registerFunctionDefinition("void EntitiesWithComponentsCallBack(Entity)");

	// Scene
//	scriptingEngine_->registerObjectType("Scene", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod("Scene", "string getName() const", asMETHODPR(Scene, getName, () const, std::string));
	scriptingEngine_->registerClassMethod("Scene", "bool visible() const", asMETHOD(Scene, visible));
	scriptingEngine_->registerClassMethod("Scene", "void setVisible(const bool)", asMETHOD(Scene, setVisible));
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
	scriptingEngine_->registerClassMethod("Scene", "CrowdHandle createCrowd(const NavigationMeshHandle& in, const CrowdConfig& in)", asMETHOD(Scene, createCrowd));
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
	scriptingEngine_->registerClassMethod("Scene", "void resetMoveTarget(const CrowdHandle& in, const AgentHandle& in)", asMETHOD(Scene, resetMoveTarget));
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void requestMoveVelocity(const CrowdHandle& in, const AgentHandle& in, const vec3& in)",
		asMETHODPR(Scene, requestMoveVelocity, (const pathfinding::CrowdHandle&, const pathfinding::AgentHandle&, const glm::vec3&), void )
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
		"RenderableHandle createRenderable(const ModelHandle& in, const vec3& in, const quat& in, const vec3& in = vec3(1.0f))",
		asMETHODPR(Scene, createRenderable, (const ModelHandle&, const glm::vec3&, const glm::quat&, const glm::vec3&), graphics::RenderableHandle)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"RenderableHandle createRenderable(const MeshHandle& in, const TextureHandle& in, const vec3& in, const quat& in, const vec3& in = vec3(1.0f))",
		asMETHODPR(Scene, createRenderable, (const graphics::MeshHandle&, const graphics::TextureHandle&, const glm::vec3&, const glm::quat&, const glm::vec3&), graphics::RenderableHandle)
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
		"ITerrain@ testCreateTerrain(HeightMap, SplatMap, DisplacementMap, CollisionShapeHandle, PolygonMeshHandle, NavigationMeshHandle)",
		asMETHOD(Scene, testCreateTerrain)
	);
	scriptingEngine_->registerClassMethod(
		"Scene",
		"void entitiesWithComponentsScriptObjectComponent(EntitiesWithComponentsCallBack@)",
		asMETHODPR(Scene, entitiesWithComponents<ecs::ScriptObjectComponent>, (void*), void)
	);
	scriptingEngine_->registerClassMethod("Scene", "void addPreSerializeCallback(PreSerializeCallback@)", asMETHODPR(Scene, addPreSerializeCallback, (void*), void));
	scriptingEngine_->registerClassMethod("Scene", "void addPostSerializeCallback(PostSerializeCallback@)", asMETHODPR(Scene, addPostSerializeCallback, (void*), void));
	scriptingEngine_->registerClassMethod("Scene", "void addPreDeserializeCallback(PreDeserializeCallback@)", asMETHODPR(Scene, addPreDeserializeCallback, (void*), void));
	scriptingEngine_->registerClassMethod("Scene", "void addPostDeserializeCallback(PostDeserializeCallback@)", asMETHODPR(Scene, addPostDeserializeCallback, (void*), void));
	scriptingEngine_->registerClassMethod("Scene", "void serialize(const string& in)", asMETHODPR(Scene, serialize, (const std::string&), void));
	scriptingEngine_->registerClassMethod("Scene", "void deserialize(const string& in)", asMETHODPR(Scene, deserialize, (const std::string&), void));
	scriptingEngine_->registerClassMethod("Scene", "Entity createEntity()", asMETHODPR(Scene, createEntity, (), ecs::Entity));
	scriptingEngine_->registerClassMethod("Scene", "void destroy(Entity& in)", asMETHODPR(Scene, destroy, (ecs::Entity&), void));
	scriptingEngine_->registerClassMethod("Scene", "void destroyAsync(Entity& in)", asMETHOD(Scene, destroyAsync));
	scriptingEngine_->registerClassMethod("Scene", "uint32 getNumEntities()", asMETHODPR(Scene, getNumEntities, () const, uint32));
	scriptingEngine_->registerClassMethod("Scene", "Raycast raycast(const Ray& in)", asMETHOD(Scene, raycast));
	scriptingEngine_->registerClassMethod("Scene", "vectorEntity query(const vec3& in, const vectorVec3& in)", asMETHODPR(Scene, query, (const glm::vec3&, const std::vector<glm::vec3>&), std::vector<ecs::Entity>));
	scriptingEngine_->registerClassMethod("Scene", "vectorEntity query(const vec3& in, const float)", asMETHODPR(Scene, query, (const glm::vec3&, const float32), std::vector<ecs::Entity>));
}
	
};
