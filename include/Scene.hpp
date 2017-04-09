#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <string>

#include <entityx/entityx.h>

#include "Types.hpp"

#include "IScene.hpp"

#include "physics/IPhysicsEngine.hpp"

#include "IGameEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"
#include "IThreadPool.hpp"
#include "IOpenGlLoader.hpp"

namespace hercules
{

class Scene : public IScene
{
public:
	Scene(
		const std::string& name,
		IGameEngine* gameEngine,
		graphics::IGraphicsEngine* graphicsEngine,
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger,
		IThreadPool* threadPool,
		IOpenGlLoader* openGlLoader
	);
	virtual ~Scene();

	virtual void tick(const float32 elapsedTime) override;
	
	virtual std::string getName() const override;
	
	virtual entities::Entity createEntity() override;
	
	virtual void assign(const entities::Entity& entity, const entities::GraphicsComponent& component) override;
	virtual void assign(const entities::Entity& entity, const entities::PhysicsComponent& component) override;
	
	virtual void rotate(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) override;
	virtual void rotate(const entities::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo = graphics::TransformSpace::TS_LOCAL) override;
	virtual void rotation(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis) override;
	virtual void rotation(const entities::Entity& entity, const glm::quat& orientation) override;
	virtual void translate(const entities::Entity& entity, const glm::vec3& translate) override;
	virtual void scale(const entities::Entity& entity, const float32 scale) override;
	virtual void scale(const entities::Entity& entity, const glm::vec3& scale) override;
	virtual void scale(const entities::Entity& entity, const float32 x, const float32 y, const float32 z) override;
	virtual void lookAt(const entities::Entity& entity, const glm::vec3& lookAt) override;
	
	virtual void position(const entities::Entity& entity, const glm::vec3& position) override;
	virtual void position(const entities::Entity& entity, const float32 x, const float32 y, const float32 z) override;
	
private:
	std::string name_;
	IGameEngine* gameEngine_;
	graphics::IGraphicsEngine* graphicsEngine_;
	utilities::Properties* properties_;
	fs::IFileSystem* fileSystem_;
	logger::ILogger* logger_;
	IThreadPool* threadPool_;
	IOpenGlLoader* openGlLoader_;
	
	std::unique_ptr< physics::IPhysicsEngine > physicsEngine_;
	
	// Entity system
	entityx::EntityX entityx_;
	std::vector<entities::Entity> entities_;
};

}

#endif /* SCENE_H_ */
