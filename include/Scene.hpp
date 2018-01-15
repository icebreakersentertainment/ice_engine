#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <string>

#include "Types.hpp"

#include "IScene.hpp"

#include "physics/IPhysicsEngine.hpp"
#include "pathfinding/IPathfindingEngine.hpp"
#include "entities/EntityComponentSystem.hpp"

#include "IGameEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"
#include "IThreadPool.hpp"
#include "IOpenGlLoader.hpp"

namespace ice_engine
{

class Scene : public IScene
{
public:
	Scene(
		const std::string& name,
		IGameEngine* gameEngine,
		audio::IAudioEngine* audioEngine,
		graphics::IGraphicsEngine* graphicsEngine,
		physics::IPhysicsEngine* physicsEngine,
		pathfinding::IPathfindingEngine* pathfindingEngine,
		scripting::IScriptingEngine* scriptingEngine,
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger,
		IThreadPool* threadPool,
		IOpenGlLoader* openGlLoader
	);
	virtual ~Scene();

	virtual void tick(const float32 delta) override;
	virtual void render() override;
	
	virtual void setISceneThingyInstance(scripting::ScriptObjectHandle scriptObjectHandle) override;
	
	virtual physics::CollisionShapeHandle createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant) override;
	virtual physics::CollisionShapeHandle createStaticBoxShape(const glm::vec3& dimensions) override;
	virtual void destroyStaticShape(const physics::CollisionShapeHandle& collisionShapeHandle) override;
	virtual void destroyAllStaticShapes() override;
	
	virtual physics::RigidBodyObjectHandle createDynamicRigidBodyObject(const physics::CollisionShapeHandle& collisionShapeHandle) override;
	virtual physics::RigidBodyObjectHandle createDynamicRigidBodyObject(
		const physics::CollisionShapeHandle& collisionShapeHandle,
		const float32 mass,
		const float32 friction,
		const float32 restitution
	) override;
	virtual physics::RigidBodyObjectHandle createDynamicRigidBodyObject(
		const physics::CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const float32 mass = 1.0f,
		const float32 friction = 1.0f,
		const float32 restitution = 1.0f
	) override;
	virtual physics::RigidBodyObjectHandle createStaticRigidBodyObject(const physics::CollisionShapeHandle& collisionShapeHandle) override;
	virtual physics::RigidBodyObjectHandle createStaticRigidBodyObject(
		const physics::CollisionShapeHandle& collisionShapeHandle,
		const float32 friction,
		const float32 restitution
	) override;
	virtual physics::RigidBodyObjectHandle createStaticRigidBodyObject(
		const physics::CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation,
		const float32 friction = 1.0f,
		const float32 restitution = 1.0f
	) override;
	virtual physics::GhostObjectHandle createGhostObject(
		const physics::CollisionShapeHandle& collisionShapeHandle
	) override;
	virtual physics::GhostObjectHandle createGhostObject(
		const physics::CollisionShapeHandle& collisionShapeHandle,
		const glm::vec3& position,
		const glm::quat& orientation
	) override;
	
	virtual graphics::RenderableHandle createRenderable(const ModelHandle& modelHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name = std::string()) override;
	virtual graphics::RenderableHandle createRenderable(const graphics::MeshHandle& meshHandle, const graphics::TextureHandle& textureHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name = std::string()) override;
	
	virtual audio::SoundSourceHandle play(const audio::SoundHandle& soundHandle, const glm::vec3& position) override;
	
	virtual graphics::PointLightHandle createPointLight(const glm::vec3& position) override;
	
	virtual std::string getName() const override;
	
	virtual const SceneStatistics& getSceneStatistics() const override;
	
	virtual entities::Entity createEntity() override;
	virtual void destroyEntity(const entities::Entity& entity) override;
	virtual uint32 getNumEntities() const override;
	
	virtual Raycast raycast(const ray::Ray& ray) override;
	
	virtual void assign(const entities::Entity& entity, const entities::GraphicsComponent& component) override;
	virtual void assign(const entities::Entity& entity, const entities::RigidBodyObjectComponent& component) override;
	virtual void assign(const entities::Entity& entity, const entities::GhostObjectComponent& component) override;
	virtual void assign(const entities::Entity& entity, const entities::PositionOrientationComponent& component) override;
	virtual void assign(const entities::Entity& entity, const entities::PointLightComponent& component) override;
	
	virtual void rotate(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) override;
	virtual void rotate(const entities::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) override;
	virtual void rotation(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis) override;
	virtual void rotation(const entities::Entity& entity, const glm::quat& orientation) override;
	virtual glm::quat rotation(const entities::Entity& entity) const override;
	virtual void translate(const entities::Entity& entity, const glm::vec3& translate) override;
	virtual void scale(const entities::Entity& entity, const float32 scale) override;
	virtual void scale(const entities::Entity& entity, const glm::vec3& scale) override;
	virtual void scale(const entities::Entity& entity, const float32 x, const float32 y, const float32 z) override;
	virtual glm::vec3 scale(const entities::Entity& entity) const override;
	virtual void lookAt(const entities::Entity& entity, const glm::vec3& lookAt) override;
	
	virtual void position(const entities::Entity& entity, const glm::vec3& position) override;
	virtual void position(const entities::Entity& entity, const float32 x, const float32 y, const float32 z) override;
	virtual glm::vec3 position(const entities::Entity& entity) const override;
	
private:
	std::string name_;
	IGameEngine* gameEngine_;
	audio::IAudioEngine* audioEngine_;
	graphics::IGraphicsEngine* graphicsEngine_;
	physics::IPhysicsEngine* physicsEngine_;
	pathfinding::IPathfindingEngine* pathfindingEngine_;
	scripting::IScriptingEngine* scriptingEngine_;
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	IThreadPool* threadPool_;
	IOpenGlLoader* openGlLoader_;
	
	audio::AudioSceneHandle audioSceneHandle_;
	graphics::RenderSceneHandle renderSceneHandle_;
	physics::PhysicsSceneHandle physicsSceneHandle_;
	pathfinding::PathfindingSceneHandle pathfindingSceneHandle_;
	scripting::ExecutionContextHandle executionContextHandle_;
	
	scripting::ScriptObjectHandle scriptObjectHandle_;
	
	SceneStatistics sceneStatistics_;
	
	// Entity system
	entities::EntityComponentSystem entityComponentSystem_;
	std::vector<entities::Entity> entities_;
	
	void initialize();
	void destroy();
	
	void addMotionChangeListener(const entities::Entity& entity);
	void removeMotionChangeListener(const entities::Entity& entity);
	
	void addUserData(const entities::Entity& entity, const entities::RigidBodyObjectComponent& rigidBodyObjectComponent);
	void addUserData(const entities::Entity& entity, const entities::GhostObjectComponent& ghostObjectComponent);
	void removeUserData(const entities::Entity& entity, const entities::RigidBodyObjectComponent& rigidBodyObjectComponent);
	void removeUserData(const entities::Entity& entity, const entities::GhostObjectComponent& ghostObjectComponent);
};

}

#endif /* SCENE_H_ */
