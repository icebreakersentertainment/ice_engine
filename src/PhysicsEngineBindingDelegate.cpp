#include <string>

#include "Platform.hpp"
#include "Types.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "PhysicsEngineBindingDelegate.hpp"
#include "BindingDelegateUtilities.hpp"

#include "scripting/IScriptingEngine.hpp"

#include "GameEngine.hpp"

namespace ice_engine
{

PhysicsEngineBindingDelegate::PhysicsEngineBindingDelegate(logger::ILogger* logger, scripting::IScriptingEngine* scriptingEngine, GameEngine* gameEngine, physics::IPhysicsEngine* physicsEngine)
	:
	logger_(logger),
	scriptingEngine_(scriptingEngine),
	gameEngine_(gameEngine),
	physicsEngine_(physicsEngine)
{
}

void PhysicsEngineBindingDelegate::bind()
{
	registerHandleBindings<physics::PhysicsSceneHandle>(scriptingEngine_, "PhysicsSceneHandle");
	registerHandleBindings<physics::CollisionShapeHandle>(scriptingEngine_, "CollisionShapeHandle");
	
	scriptingEngine_->registerObjectType("RigidBodyObjectHandle", sizeof(physics::RigidBodyObjectHandle), asOBJ_VALUE | asOBJ_APP_CLASS_ALLINTS | asOBJ_POD | asGetTypeTraits<physics::RigidBodyObjectHandle>());
	//scriptingEngine_->registerClassMethod("RigidBodyObjectHandle", "uint64 id() const", asMETHODPR(physics::RigidBodyObjectHandle, id, () const, uint64));
	//scriptingEngine_->registerClassMethod("RigidBodyObjectHandle", "uint32 index() const", asMETHODPR(physics::RigidBodyObjectHandle, index, () const, uint32));
	//scriptingEngine_->registerClassMethod("RigidBodyObjectHandle", "uint32 version() const", asMETHODPR(physics::RigidBodyObjectHandle, version, () const, uint32));
	scriptingEngine_->registerObjectType("GhostObjectHandle", sizeof(physics::GhostObjectHandle), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLINTS | asGetTypeTraits<physics::GhostObjectHandle>());
	//scriptingEngine_->registerClassMethod("GhostObjectHandle", "uint64 id() const", asMETHODPR(physics::GhostObjectHandle, id, () const, uint64));
	//scriptingEngine_->registerClassMethod("GhostObjectHandle", "uint32 index() const", asMETHODPR(physics::GhostObjectHandle, index, () const, uint32));
	//scriptingEngine_->registerClassMethod("GhostObjectHandle", "uint32 version() const", asMETHODPR(physics::GhostObjectHandle, version, () const, uint32));
	
	// IPhysicsEngine
	scriptingEngine_->registerObjectType("IPhysicsEngine", 0, asOBJ_REF | asOBJ_NOCOUNT);
	scriptingEngine_->registerGlobalProperty("IPhysicsEngine physics", physicsEngine_);
	scriptingEngine_->registerClassMethod(
		"IPhysicsEngine",
		"CollisionShapeHandle createStaticPlaneShape(const vec3& in, const float)",
		asMETHODPR(physics::IPhysicsEngine, createStaticPlaneShape, (const glm::vec3&, const float32), physics::CollisionShapeHandle)
	);
	scriptingEngine_->registerClassMethod(
		"IPhysicsEngine",
		"RigidBodyObjectHandle createStaticRigidBodyObject(const PhysicsSceneHandle& in, const CollisionShapeHandle& in)",
		asMETHODPR(physics::IPhysicsEngine, createStaticRigidBodyObject, (const physics::PhysicsSceneHandle&, const physics::CollisionShapeHandle&, const physics::UserData&), physics::RigidBodyObjectHandle)
	);
}
	
};
