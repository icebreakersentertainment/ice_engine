#include "Scene.hpp"

#include "HerculesMotionChangeListener.hpp"

namespace hercules
{

Scene::Scene(
		const std::string& name,
		IGameEngine* gameEngine,
		graphics::IGraphicsEngine* graphicsEngine,
		physics::IPhysicsEngine* physicsEngine,
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger,
		IThreadPool* threadPool,
		IOpenGlLoader* openGlLoader
	)
	:
		name_(name),
		gameEngine_(gameEngine),
		graphicsEngine_(graphicsEngine),
		physicsEngine_(physicsEngine),
		properties_(properties),
		fileSystem_(fileSystem),
		logger_(logger),
		threadPool_(threadPool),
		openGlLoader_(openGlLoader)
{
}

Scene::~Scene()
{
}

void Scene::tick(const float32 elapsedTime)
{
	physicsEngine_->tick(elapsedTime);
}

physics::CollisionShapeHandle Scene::createStaticPlaneShape(const glm::vec3& planeNormal, const float32 planeConstant)
{
	return physicsEngine_->createStaticPlaneShape(planeNormal, planeConstant);
}

physics::CollisionShapeHandle Scene::createStaticBoxShape(const glm::vec3& dimensions)
{
	return physicsEngine_->createStaticBoxShape(dimensions);
}

void Scene::destroyStaticShape(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return physicsEngine_->destroyStaticShape(collisionShapeHandle);
}

void Scene::destroyAllStaticShapes()
{
	return physicsEngine_->destroyAllStaticShapes();
}

physics::CollisionBodyHandle Scene::createDynamicRigidBody(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return physicsEngine_->createDynamicRigidBody(collisionShapeHandle);
}

physics::CollisionBodyHandle Scene::createDynamicRigidBody(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const float32 mass,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createDynamicRigidBody(collisionShapeHandle, mass, friction, restitution);
}

physics::CollisionBodyHandle Scene::createDynamicRigidBody(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 mass,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createDynamicRigidBody(collisionShapeHandle, position, orientation, mass, friction, restitution);
}

physics::CollisionBodyHandle Scene::createStaticRigidBody(const physics::CollisionShapeHandle& collisionShapeHandle)
{
	return physicsEngine_->createStaticRigidBody(collisionShapeHandle);
}

physics::CollisionBodyHandle Scene::createStaticRigidBody(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createStaticRigidBody(collisionShapeHandle, friction, restitution);
}

physics::CollisionBodyHandle Scene::createStaticRigidBody(
	const physics::CollisionShapeHandle& collisionShapeHandle,
	const glm::vec3& position,
	const glm::quat& orientation,
	const float32 friction,
	const float32 restitution
)
{
	return physicsEngine_->createStaticRigidBody(collisionShapeHandle, position, orientation, friction, restitution);
}

void Scene::addMotionChangeListener(const entities::Entity& entity)
{
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	std::unique_ptr<HerculesMotionChangeListener> motionChangeListener = std::make_unique<HerculesMotionChangeListener>(entity, this);
	
	physicsEngine_->setMotionChangeListener(physicsComponent->collisionBodyHandle, std::move(motionChangeListener));
}

void Scene::removeMotionChangeListener(const entities::Entity& entity)
{
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	physicsEngine_->setMotionChangeListener(physicsComponent->collisionBodyHandle, nullptr);
}

graphics::RenderableHandle Scene::createRenderable(const ModelHandle& modelHandle, const graphics::ShaderProgramHandle& shaderProgramHandle, const std::string& name)
{
	return gameEngine_->createRenderable(modelHandle, shaderProgramHandle, name);
}

std::string Scene::getName() const
{
	return name_;
}

entities::Entity Scene::createEntity()
{
	entityx::Entity e = entityComponentSystem_.entities.create();
	
	logger_->debug( "Created entity with id: " + std::to_string(e.id().id()) );
	
	return entities::Entity( e.id().id() );
}

void Scene::destroyEntity(const entities::Entity& entity)
{
	entityComponentSystem_.entities.destroy(static_cast<entityx::Entity::Id>(entity.getId()));
}

uint32 Scene::getNumEntities() const
{
	return entityComponentSystem_.entities.size();
}

void Scene::assign(const entities::Entity& entity, const entities::GraphicsComponent& component)
{
	entityComponentSystem_.entities.assign<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()), std::forward<const entities::GraphicsComponent>(component));
}

void Scene::assign(const entities::Entity& entity, const entities::PhysicsComponent& component)
{
	entityComponentSystem_.entities.assign<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()), std::forward<const entities::PhysicsComponent>(component));
	
	addMotionChangeListener(entity);
}

void Scene::assign(const entities::Entity& entity, const entities::PositionOrientationComponent& component)
{
	entityComponentSystem_.entities.assign<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()), std::forward<const entities::PositionOrientationComponent>(component));
}

void Scene::rotate(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	switch( relativeTo )
	{
		case graphics::TransformSpace::TS_LOCAL:
			component->orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) ) * component->orientation;
			break;
		
		case graphics::TransformSpace::TS_WORLD:
			component->orientation =  component->orientation * glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
			break;
			
		default:
			throw std::runtime_error(std::string("Invalid TransformSpace type."));
	}
	
	graphicsEngine_->rotation(graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsComponent->collisionBodyHandle, component->orientation);
}

void Scene::rotate(const entities::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	switch( relativeTo )
	{
		case graphics::TransformSpace::TS_LOCAL:
			component->orientation = component->orientation * glm::normalize( orientation );
			break;
		
		case graphics::TransformSpace::TS_WORLD:
			component->orientation =  glm::normalize( orientation ) * component->orientation;
			break;
			
		default:
			throw std::runtime_error(std::string("Invalid TransformSpace type."));
	}
	
	graphicsEngine_->rotation(graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsComponent->collisionBodyHandle, component->orientation);
}

void Scene::rotation(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	component->orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
	
	graphicsEngine_->rotation(graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsComponent->collisionBodyHandle, component->orientation);
}

void Scene::rotation(const entities::Entity& entity, const glm::quat& orientation)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	component->orientation = glm::normalize( orientation );
	
	graphicsEngine_->rotation(graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsComponent->collisionBodyHandle, component->orientation);
}

glm::quat Scene::rotation(const entities::Entity& entity)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	return component->orientation;
}

void Scene::translate(const entities::Entity& entity, const glm::vec3& translate)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	component->position += translate;
	
	graphicsEngine_->position(graphicsComponent->renderableHandle, component->position);
	physicsEngine_->position(physicsComponent->collisionBodyHandle, component->position);
}

void Scene::scale(const entities::Entity& entity, const float32 scale)
{
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	component->scale = glm::vec3(scale, scale, scale);
	
	graphicsEngine_->scale(component->renderableHandle, scale);
}

void Scene::scale(const entities::Entity& entity, const glm::vec3& scale)
{
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	component->scale = scale;
	
	graphicsEngine_->scale(component->renderableHandle, scale);
}

void Scene::scale(const entities::Entity& entity, const float32 x, const float32 y, const float32 z)
{
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	component->scale = glm::vec3(x, y, z);
	
	graphicsEngine_->scale(component->renderableHandle, x, y, z);
}

glm::vec3 Scene::scale(const entities::Entity& entity)
{
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	return component->scale;
}

void Scene::lookAt(const entities::Entity& entity, const glm::vec3& lookAt)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	const glm::mat4 lookAtMatrix = glm::lookAt(component->position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	component->orientation =  glm::normalize( component->orientation * glm::quat_cast( lookAtMatrix ) );
	
	graphicsEngine_->rotation(graphicsComponent->renderableHandle, component->orientation);
	physicsEngine_->rotation(physicsComponent->collisionBodyHandle, component->orientation);
}

void Scene::position(const entities::Entity& entity, const glm::vec3& position)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	component->position = position;
	
	graphicsEngine_->position(graphicsComponent->renderableHandle, position);
	physicsEngine_->position(physicsComponent->collisionBodyHandle, position);
}

void Scene::position(const entities::Entity& entity, const float32 x, const float32 y, const float32 z)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto physicsComponent = entityComponentSystem_.entities.component<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	component->position = glm::vec3(x, y, z);
	
	graphicsEngine_->position(graphicsComponent->renderableHandle, x, y, z);
	physicsEngine_->position(physicsComponent->collisionBodyHandle, x, y, z);
}

glm::vec3 Scene::position(const entities::Entity& entity)
{
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	return component->position;
}

}
