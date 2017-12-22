#include <string>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Platform.hpp"
#include "Types.hpp"

#include "ModelHandle.hpp"
#include "IScene.hpp"
#include "IKeyboardEventListener.hpp"
#include "IMouseMotionEventListener.hpp"
#include "IMouseButtonEventListener.hpp"
#include "IMouseWheelEventListener.hpp"

#include "graphics/IGraphicsEngine.hpp"

#include "graphics/gui/IGui.hpp"

#include "graphics/model/Model.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "pathfinding/IPathfindingEngine.hpp"

#include "ScriptingEngineBindingDelegate.hpp"
#include "AudioEngineBindingDelegate.hpp"
#include "GraphicsEngineBindingDelegate.hpp"
#include "PhysicsEngineBindingDelegate.hpp"
#include "PathfindingEngineBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "scripting/IScriptingEngine.hpp"

//#include "scripting/angel_script/scriptvector/scriptvector.hpp"

#include "GameEngine.hpp"

#include "noise/Noise.hpp"

namespace ice_engine
{

namespace idebugrenderer
{
void pushLine(const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, IDebugRenderer* debugRenderer)
{
	debugRenderer->pushLine(from, to, color);
}
}

namespace raybinding
{
void InitConstructor(const glm::vec3& from, const glm::vec3& to, void* memory) { new(memory) ray::Ray(from, to); }
}

ScriptingEngineBindingDelegate::ScriptingEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, graphics::IGraphicsEngine* graphicsEngine, physics::IPhysicsEngine* physicsEngine, pathfinding::IPathfindingEngine* pathfindingEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine),
	graphicsEngine_(graphicsEngine),
	physicsEngine_(physicsEngine),
	pathfindingEngine_(pathfindingEngine)
{
}

ScriptingEngineBindingDelegate::~ScriptingEngineBindingDelegate()
{
}

void ScriptingEngineBindingDelegate::bind()
{
	scriptingEngine_->registerObjectType("Image", 0, asOBJ_REF | asOBJ_NOCOUNT);
	
	// Future bindings
	scriptingEngine_->registerEnum("future_status");
	scriptingEngine_->registerEnumValue("future_status", "deferred", static_cast<int64>(std::future_status::deferred));
	scriptingEngine_->registerEnumValue("future_status", "ready", static_cast<int64>(std::future_status::ready));
	scriptingEngine_->registerEnumValue("future_status", "timeout", static_cast<int64>(std::future_status::timeout));
	
	//auto audioEngineBindingDelegate = AudioEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, audioEngine_);
	//audioEngineBindingDelegate.bind();
	
	auto graphicsEngineBindingDelegate = GraphicsEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, graphicsEngine_);
	graphicsEngineBindingDelegate.bind();
	
	auto physicsEngineBindingDelegate = PhysicsEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, physicsEngine_);
	physicsEngineBindingDelegate.bind();
	
	//auto pathfindingEngineBindingDelegate = PathfindingEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, pathfindingEngine_);
	//pathfindingEngineBindingDelegate.bind();
	
	// Types available in the scripting engine
	registerHandleBindings<entities::Entity>(scriptingEngine_, "Entity");
	
	registerHandleBindings<ModelHandle>(scriptingEngine_, "ModelHandle");
	
	scriptingEngine_->registerObjectType("GraphicsComponent", sizeof(entities::GraphicsComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::GraphicsComponent>());
	scriptingEngine_->registerObjectProperty("GraphicsComponent", "vec3 scale", asOFFSET(entities::GraphicsComponent, scale));
	scriptingEngine_->registerObjectProperty("GraphicsComponent", "RenderableHandle renderableHandle", asOFFSET(entities::GraphicsComponent, renderableHandle));
	scriptingEngine_->registerObjectType("RigidBodyObjectComponent", sizeof(entities::RigidBodyObjectComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::RigidBodyObjectComponent>());
	scriptingEngine_->registerObjectProperty("RigidBodyObjectComponent", "RigidBodyObjectHandle rigidBodyObjectHandle", asOFFSET(entities::RigidBodyObjectComponent, rigidBodyObjectHandle));
	scriptingEngine_->registerObjectType("GhostObjectComponent", sizeof(entities::GhostObjectComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::GhostObjectComponent>());
	scriptingEngine_->registerObjectProperty("GhostObjectComponent", "GhostObjectHandle ghostObjectHandle", asOFFSET(entities::GhostObjectComponent, ghostObjectHandle));
	scriptingEngine_->registerObjectType("PositionOrientationComponent", sizeof(entities::PositionOrientationComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::PositionOrientationComponent>());
	scriptingEngine_->registerObjectProperty("PositionOrientationComponent", "vec3 position", asOFFSET(entities::PositionOrientationComponent, position));
	scriptingEngine_->registerObjectProperty("PositionOrientationComponent", "quat orientation", asOFFSET(entities::PositionOrientationComponent, orientation));
	scriptingEngine_->registerObjectType("PointLightComponent", sizeof(entities::PointLightComponent), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<entities::PointLightComponent>());
	scriptingEngine_->registerObjectProperty("PointLightComponent", "vec3 position", asOFFSET(entities::PointLightComponent, position));
	scriptingEngine_->registerObjectProperty("PointLightComponent", "PointLightHandle pointLightHandle", asOFFSET(entities::PointLightComponent, pointLightHandle));
	
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
		asMETHODPR(Raycast, setEntity, (const entities::Entity&), void)
	);
	scriptingEngine_->registerClassMethod(
		"Raycast",
		"const Entity& entity() const",
		asMETHODPR(Raycast, entity, () const, const entities::Entity&)
	);
	//scriptingEngine_->registerObjectProperty("Raycast", "vec3 from", asOFFSET(ray::Ray, from));
	//scriptingEngine_->registerObjectProperty("Raycast", "vec3 to", asOFFSET(ray::Ray, to));
	
	scriptingEngine_->registerObjectType("Noise", sizeof(noise::Noise), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<noise::Noise>());
	scriptingEngine_->registerClassMethod(
		"Noise",
		"float getValue(const float, const float) const",
		asMETHODPR(noise::Noise, getValue, (const float32, const float32) const, float32)
	);
	
	// ILogger bindings
	scriptingEngine_->registerObjectType("ILogger", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("ILogger logger", logger_);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void info(const string& in)",
		asMETHODPR(logger::ILogger, info, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void debug(const string& in)",
		asMETHODPR(logger::ILogger, debug, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void warn(const string& in)",
		asMETHODPR(logger::ILogger, warn, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void error(const string& in)",
		asMETHODPR(logger::ILogger, error, (const std::string&), void)
	);
	scriptingEngine_->registerClassMethod(
		"ILogger",
		"void fatal(const string& in)",
		asMETHODPR(logger::ILogger, fatal, (const std::string&), void)
	);
	
	// IThreadPool bindings
	scriptingEngine_->registerObjectType("IThreadPool", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getActiveWorkerCount()",
		asMETHODPR(IThreadPool, getActiveWorkerCount, () const, uint32)
	);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getInactiveWorkerCount()",
		asMETHODPR(IThreadPool, getInactiveWorkerCount, () const, uint32)
	);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getWorkQueueSize()",
		asMETHODPR(IThreadPool, getWorkQueueSize, () const, uint32)
	);
	scriptingEngine_->registerClassMethod(
		"IThreadPool",
		"int getWorkQueueCount()",
		asMETHODPR(IThreadPool, getWorkQueueCount, () const, uint32)
	);
	
	// IOpenGlLoader bindings
	scriptingEngine_->registerObjectType("IOpenGlLoader", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod(
		"IOpenGlLoader",
		"int getWorkQueueCount()",
		asMETHODPR(IOpenGlLoader, getWorkQueueCount, () const, uint32)
	);
		
	registerSharedFutureBindings<image::Image*>(scriptingEngine_, "shared_futureImage", "Image@");
	registerSharedFutureBindings<ModelHandle>(scriptingEngine_, "shared_futureModelHandle", "ModelHandle");
		
	scriptingEngine_->registerObjectType("EngineStatistics", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectProperty("EngineStatistics", "float fps", asOFFSET(EngineStatistics, fps));
	scriptingEngine_->registerObjectProperty("EngineStatistics", "float renderTime", asOFFSET(EngineStatistics, renderTime));
	scriptingEngine_->registerObjectType("SceneStatistics", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerObjectProperty("SceneStatistics", "float physicsTime", asOFFSET(SceneStatistics, physicsTime));
	scriptingEngine_->registerObjectProperty("SceneStatistics", "float renderTime", asOFFSET(SceneStatistics, renderTime));
	
	// IGame
	scriptingEngine_->registerInterface("IGame");
	scriptingEngine_->registerInterfaceMethod("IGame", "void initialize()");
	scriptingEngine_->registerInterfaceMethod("IGame", "void destroy()");
	scriptingEngine_->registerInterfaceMethod("IGame", "void tick(const float)");
	
	// ISceneThingy
	scriptingEngine_->registerInterface("ISceneThingy");
	scriptingEngine_->registerInterfaceMethod("ISceneThingy", "void preTick(const float)");
	scriptingEngine_->registerInterfaceMethod("ISceneThingy", "void postTick(const float)");
	
	// Listeners
	scriptingEngine_->registerInterface("IKeyboardEventListener");
	scriptingEngine_->registerInterfaceMethod("IKeyboardEventListener", "bool processEvent(const KeyboardEvent& in)");
	scriptingEngine_->registerInterface("IMouseMotionEventListener");
	scriptingEngine_->registerInterfaceMethod("IMouseMotionEventListener", "bool processEvent(const MouseMotionEvent& in)");
	scriptingEngine_->registerInterface("IMouseButtonEventListener");
	scriptingEngine_->registerInterfaceMethod("IMouseButtonEventListener", "bool processEvent(const MouseButtonEvent& in)");
	scriptingEngine_->registerInterface("IMouseWheelEventListener");
	scriptingEngine_->registerInterfaceMethod("IMouseWheelEventListener", "bool processEvent(const MouseWheelEvent& in)");
	
	// IScene
	scriptingEngine_->registerObjectType("IScene", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerClassMethod("IScene", "string getName() const", asMETHODPR(IScene, getName, () const, std::string));
	scriptingEngine_->registerClassMethod(
		"IScene",
		"const SceneStatistics@ getSceneStatistics()",
		asMETHODPR(IScene, getSceneStatistics, () const, const SceneStatistics&)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionShapeHandle createStaticPlaneShape(const vec3& in, const float)",
		asMETHODPR(IScene, createStaticPlaneShape, (const glm::vec3&, const float32), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"CollisionShapeHandle createStaticBoxShape(const vec3& in)",
		asMETHODPR(IScene, createStaticBoxShape, (const glm::vec3&), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void destroyStaticShape(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, destroyStaticShape, (const physics::CollisionShapeHandle&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void destroyAllStaticShapes()",
		asMETHODPR(IScene, destroyAllStaticShapes, (), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createDynamicRigidBodyObject(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, createDynamicRigidBodyObject, (const physics::CollisionShapeHandle&), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createDynamicRigidBodyObject(const CollisionShapeHandle& in, const float, const float, const float)",
		asMETHODPR(IScene, createDynamicRigidBodyObject, (const physics::CollisionShapeHandle&, const float32, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createDynamicRigidBodyObject(const CollisionShapeHandle& in, const vec3& in, const quat& in, const float, const float, const float)",
		asMETHODPR(IScene, createDynamicRigidBodyObject, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&, const float32, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createStaticRigidBodyObject(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, createStaticRigidBodyObject, (const physics::CollisionShapeHandle&), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createStaticRigidBodyObject(const CollisionShapeHandle& in, const float, const float)",
		asMETHODPR(IScene, createStaticRigidBodyObject, (const physics::CollisionShapeHandle&, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RigidBodyObjectHandle createStaticRigidBodyObject(const CollisionShapeHandle& in, const vec3& in, const quat& in, const float, const float)",
		asMETHODPR(IScene, createStaticRigidBodyObject, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&, const float32, const float32), physics::RigidBodyObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"GhostObjectHandle createGhostObject(const CollisionShapeHandle& in)",
		asMETHODPR(IScene, createGhostObject, (const physics::CollisionShapeHandle&), physics::GhostObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"GhostObjectHandle createGhostObject(const CollisionShapeHandle& in, const vec3& in, const quat& in)",
		asMETHODPR(IScene, createGhostObject, (const physics::CollisionShapeHandle&, const glm::vec3&, const glm::quat&), physics::GhostObjectHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RenderableHandle createRenderable(const ModelHandle& in, const ShaderProgramHandle& in, const string& in = string())",
		asMETHODPR(IScene, createRenderable, (const ModelHandle&, const graphics::ShaderProgramHandle&, const std::string&), graphics::RenderableHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"RenderableHandle createRenderable(const MeshHandle& in, const TextureHandle& in, const ShaderProgramHandle& in, const string& in = string())",
		asMETHODPR(IScene, createRenderable, (const graphics::MeshHandle&, const graphics::TextureHandle&, const graphics::ShaderProgramHandle&, const std::string&), graphics::RenderableHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"PointLightHandle createPointLight(const vec3& in)",
		asMETHODPR(IScene, createPointLight, (const glm::vec3&), graphics::PointLightHandle)
	);
	scriptingEngine_->registerClassMethod("IScene", "Entity createEntity()", asMETHODPR(IScene, createEntity, (), entities::Entity));
	scriptingEngine_->registerClassMethod("IScene", "uint32 getNumEntities()", asMETHODPR(IScene, getNumEntities, () const, uint32));
	scriptingEngine_->registerClassMethod(
		"IScene",
		"Raycast raycast(const Ray& in)",
		asMETHODPR(IScene, raycast, (const ray::Ray&), Raycast)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void assign(const Entity& in, const GraphicsComponent& in)", 
		asMETHODPR(IScene, assign, (const entities::Entity&, const entities::GraphicsComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void assign(const Entity& in, const RigidBodyObjectComponent& in)", 
		asMETHODPR(IScene, assign, (const entities::Entity&, const entities::RigidBodyObjectComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void assign(const Entity& in, const GhostObjectComponent& in)", 
		asMETHODPR(IScene, assign, (const entities::Entity&, const entities::GhostObjectComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void assign(const Entity& in, const PointLightComponent& in)", 
		asMETHODPR(IScene, assign, (const entities::Entity&, const entities::PointLightComponent&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void rotate(const Entity& in, const quat& in, const TransformSpace& in = TransformSpace::TS_LOCAL)",
		asMETHODPR(IScene, rotate, (const entities::Entity&, const glm::quat&, const graphics::TransformSpace&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void rotate(const Entity& in, const float, const vec3& in, const TransformSpace& in = TransformSpace::TS_LOCAL)",
		asMETHODPR(IScene, rotate, (const entities::Entity&, const float32, const glm::vec3&, const graphics::TransformSpace&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void rotation(const Entity& in, const quat& in)",
		asMETHODPR(IScene, rotation, (const entities::Entity&, const glm::quat&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"quat rotation(const Entity& in) const",
		asMETHODPR(IScene, rotation, (const entities::Entity&) const, glm::quat)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void scale(const Entity& in, const float)",
		asMETHODPR(IScene, scale, (const entities::Entity&, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void scale(const Entity& in, const vec3& in)",
		asMETHODPR(IScene, scale, (const entities::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"vec3 scale(const Entity& in) const",
		asMETHODPR(IScene, scale, (const entities::Entity&) const, glm::vec3)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void scale(const Entity& in, const float, const float, const float)",
		asMETHODPR(IScene, scale, (const entities::Entity&, const float32, const float32, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void translate(const Entity& in, const vec3& in)",
		asMETHODPR(IScene, translate, (const entities::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void position(const Entity& in, const vec3& in)",
		asMETHODPR(IScene, position, (const entities::Entity&, const glm::vec3&), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"void position(const Entity& in, const float, const float, const float)",
		asMETHODPR(IScene, position, (const entities::Entity&, const float32, const float32, const float32), void)
	);
	scriptingEngine_->registerClassMethod(
		"IScene",
		"vec3 position(const Entity& in) const",
		asMETHODPR(IScene, position, (const entities::Entity&) const, glm::vec3)
	);
	
	// IDebugRenderer
	scriptingEngine_->registerObjectType("IDebugRenderer", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IDebugRenderer debugRenderer", gameEngine_->getDebugRenderer());
	scriptingEngine_->registerObjectMethod(
		"IDebugRenderer",
		"void pushLine(const vec3& in, const vec3& in, const vec3& in)",
		asFUNCTION(idebugrenderer::pushLine),
		asCALL_CDECL_OBJLAST
	);
	
	// IGameEngine functions available in the scripting engine
	scriptingEngine_->registerGlobalFunction(
		"const EngineStatistics@ getEngineStatistics()",
		asMETHODPR(IGameEngine, getEngineStatistics, () const, const EngineStatistics&),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void setIGameInstance(IGame@)",
		asMETHODPR(GameEngine, setIGameInstance, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IGraphicsEngine@ getGraphicsEngine()",
		asMETHODPR(IGameEngine, getGraphicsEngine, () const, graphics::IGraphicsEngine*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IPhysicsEngine@ getPhysicsEngine()",
		asMETHODPR(IGameEngine, getPhysicsEngine, () const, physics::IPhysicsEngine*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IDebugRenderer@ getDebugRenderer()",
		asMETHODPR(IGameEngine, getDebugRenderer, () const, IDebugRenderer*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IThreadPool@ getBackgroundThreadPool()",
		asMETHODPR(IGameEngine, getBackgroundThreadPool, () const, IThreadPool*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IThreadPool@ getForegroundThreadPool()",
		asMETHODPR(IGameEngine, getForegroundThreadPool, () const, IThreadPool*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IOpenGlLoader@ getOpenGlLoader()",
		asMETHODPR(IGameEngine, getOpenGlLoader, () const, IOpenGlLoader*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"IGui@ createGui(const string& in)",
		asMETHODPR(IGameEngine, createGui, (const std::string&), graphics::gui::IGui*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyGui(const IGui@)",
		asMETHODPR(IGameEngine, destroyGui, (const graphics::gui::IGui*), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addKeyboardEventListener(IKeyboardEventListener@)",
		asMETHODPR(GameEngine, addKeyboardEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMouseMotionEventListener(IMouseMotionEventListener@)",
		asMETHODPR(GameEngine, addMouseMotionEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMouseButtonEventListener(IMouseButtonEventListener@)",
		asMETHODPR(GameEngine, addMouseButtonEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void addMouseWheelEventListener(IMouseWheelEventListener@)",
		asMETHODPR(GameEngine, addMouseWheelEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeKeyboardEventListener(IKeyboardEventListener@)",
		asMETHODPR(GameEngine, removeKeyboardEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMouseMotionEventListener(IMouseMotionEventListener@)",
		asMETHODPR(GameEngine, removeMouseMotionEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMouseButtonEventListener(IMouseButtonEventListener@)",
		asMETHODPR(GameEngine, addMouseButtonEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void removeMouseWheelEventListener(IMouseWheelEventListener@)",
		asMETHODPR(GameEngine, addMouseWheelEventListener, (scripting::ScriptObjectHandle), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Model@ importModel(const string& in, const string& in)",
		asMETHODPR(IGameEngine, importModel, (const std::string&, const std::string&), graphics::model::Model*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureModel importModelAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, importModelAsync, (const std::string&, const std::string&), std::shared_future<graphics::model::Model*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Image@ loadImage(const string& in, const string& in)",
		asMETHODPR(IGameEngine, loadImage, (const std::string&, const std::string&), image::Image*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureImage loadImageAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, loadImageAsync, (const std::string&, const std::string&), std::shared_future<image::Image*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Model@ getModel(const string& in)",
		asMETHODPR(IGameEngine, getModel, (const std::string&) const, graphics::model::Model*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"Image@ getImage(const string& in)",
		asMETHODPR(IGameEngine, getImage, (const std::string&) const, image::Image*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadModel(const string& in)",
		asMETHODPR(IGameEngine, unloadModel, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void unloadImage(const string& in)",
		asMETHODPR(IGameEngine, unloadImage, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ModelHandle loadStaticModel(Model@)",
		asMETHODPR(IGameEngine, loadStaticModel, (const graphics::model::Model*), ModelHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureModelHandle loadStaticModelAsync(Model@)",
		asMETHODPR(IGameEngine, loadStaticModelAsync, (const graphics::model::Model*), std::shared_future<ModelHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"VertexShaderHandle createVertexShader(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createVertexShader, (const std::string&, const std::string&), graphics::VertexShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureVertexShaderHandle createVertexShaderAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createVertexShaderAsync, (const std::string&, const std::string&), std::shared_future<graphics::VertexShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"VertexShaderHandle createVertexShaderFromSource(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createVertexShaderFromSource, (const std::string&, const std::string&), graphics::VertexShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureVertexShaderHandle createVertexShaderFromSourceAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createVertexShaderFromSourceAsync, (const std::string&, const std::string&), std::shared_future<graphics::VertexShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"FragmentShaderHandle createFragmentShader(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createFragmentShader, (const std::string&, const std::string&), graphics::FragmentShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureFragmentShaderHandle createFragmentShaderAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createFragmentShaderAsync, (const std::string&, const std::string&), std::shared_future<graphics::FragmentShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"FragmentShaderHandle createFragmentShaderFromSource(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createFragmentShaderFromSource, (const std::string&, const std::string&), graphics::FragmentShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureFragmentShaderHandle createFragmentShaderFromSourceAsync(const string& in, const string& in)",
		asMETHODPR(IGameEngine, createFragmentShaderFromSourceAsync, (const std::string&, const std::string&), std::shared_future<graphics::FragmentShaderHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"VertexShaderHandle getVertexShader(const string& in)",
		asMETHODPR(IGameEngine, getVertexShader, (const std::string&) const, graphics::VertexShaderHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShader(const string& in)",
		asMETHODPR(IGameEngine, destroyShader, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShader(const VertexShaderHandle& in)",
		asMETHODPR(IGameEngine, destroyShader, (const graphics::VertexShaderHandle&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShader(const FragmentShaderHandle& in)",
		asMETHODPR(IGameEngine, destroyShader, (const graphics::FragmentShaderHandle&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderProgramHandle createShaderProgram(const string& in, const VertexShaderHandle& in, const FragmentShaderHandle& in)",
		asMETHODPR(IGameEngine, createShaderProgram, (const std::string&, const graphics::VertexShaderHandle&, const graphics::FragmentShaderHandle&), graphics::ShaderProgramHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"shared_futureShaderProgramHandle createShaderProgramAsync(const string& in, const VertexShaderHandle& in, const FragmentShaderHandle& in)",
		asMETHODPR(IGameEngine, createShaderProgramAsync, (const std::string&, const graphics::VertexShaderHandle&, const graphics::FragmentShaderHandle&), std::shared_future<graphics::ShaderProgramHandle>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"ShaderProgramHandle getShaderProgram(const string& in)",
		asMETHODPR(IGameEngine, getShaderProgram, (const std::string&) const, graphics::ShaderProgramHandle),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShaderProgram(const string& in)",
		asMETHODPR(IGameEngine, destroyShaderProgram, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyShaderProgram(const ShaderProgramHandle& in)",
		asMETHODPR(IGameEngine, destroyShaderProgram, (const graphics::ShaderProgramHandle&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	scriptingEngine_->registerGlobalFunction(
		"IScene@ createScene(const string& in)",
		asMETHODPR(IGameEngine, createScene, (const std::string&), IScene*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	scriptingEngine_->registerGlobalFunction(
		"IScene@ getScene(const string& in)",
		asMETHODPR(IGameEngine, getScene, (const std::string&) const, IScene*),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	scriptingEngine_->registerGlobalFunction(
		"void destroyScene(const string& in)",
		asMETHODPR(IGameEngine, destroyScene, (const std::string&), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	scriptingEngine_->registerGlobalFunction(
		"void destroyScene(IScene@)",
		asMETHODPR(IGameEngine, destroyScene, (IScene* scene), void),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	registerVectorBindings<IScene*>(scriptingEngine_, "vectorIScene", "IScene@");
	scriptingEngine_->registerGlobalFunction(
		"vectorIScene getAllScenes()",
		asMETHODPR(IGameEngine, getAllScenes, () const, std::vector<IScene*>),
		asCALL_THISCALL_ASGLOBAL,
		gameEngine_
	);
	
	// CHEATING
	auto pathfindingEngineBindingDelegate = PathfindingEngineBindingDelegate(logger_, scriptingEngine_, gameEngine_, pathfindingEngine_);
	pathfindingEngineBindingDelegate.bind();
}
	
};
