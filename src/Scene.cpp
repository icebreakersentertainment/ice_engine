#include "Scene.hpp"

#include "HerculesMotionStateListener.hpp"

#include "physics/PhysicsFactory.hpp"

#include "utilities/ImageLoader.hpp"

namespace hercules
{

Scene::Scene(
		const std::string& name,
		IGameEngine* gameEngine,
		graphics::IGraphicsEngine* graphicsEngine,
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
		properties_(properties),
		fileSystem_(fileSystem),
		logger_(logger),
		threadPool_(threadPool),
		openGlLoader_(openGlLoader)
{
	logger_->info( "initialize physics for scene." );
	physicsEngine_ = physics::PhysicsFactory::createPhysicsEngine(properties_, fileSystem_, logger_);
	
	{
		std::vector<glm::vec3> vertices;
		
		/*
		vertices.push_back( glm::vec3(0.5f, 0.0f, 0.5f) );
		vertices.push_back( glm::vec3(0.5f, 0.0f, -0.5f) );
		vertices.push_back( glm::vec3(-0.5f, 0.0f, -0.5f) );
		vertices.push_back( glm::vec3(-0.5f, 0.0f, 0.5f) );
		*/
		
		vertices.push_back( glm::vec3(0.5f,  0.5f, 0.0f) );
		vertices.push_back( glm::vec3(0.5f, -0.5f, 0.0f) );
		vertices.push_back( glm::vec3(-0.5f, -0.5f, 0.0f) );
		vertices.push_back( glm::vec3(-0.5f,  0.5f, 0.0f) );
		
		std::vector<uint32> indices;
		// First triangle
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(3);
		// Second triangle
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
		
		std::vector<glm::vec4> colors;
		
		colors.push_back( glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) );
		colors.push_back( glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) );
		colors.push_back( glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) );
		colors.push_back( glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) );
		
		std::vector<glm::vec3> normals;
		
		normals.push_back( glm::vec3(0.5f,  0.5f, 0.0f) );
		normals.push_back( glm::vec3(0.5f, -0.5f, 0.0f) );
		normals.push_back( glm::vec3(-0.5f, -0.5f, 0.0f) );
		normals.push_back( glm::vec3(-0.5f,  0.5f, 0.0f) );
		
		std::vector<glm::vec2> textureCoordinates;
		
		textureCoordinates.push_back( glm::vec2(1.0f, 1.0f) );
		textureCoordinates.push_back( glm::vec2(1.0f, 0.0f) );
		textureCoordinates.push_back( glm::vec2(0.0f, 0.0f) );
		textureCoordinates.push_back( glm::vec2(0.0f, 1.0f) );
		
		auto il = utilities::ImageLoader(logger_);
		auto image = il.loadImageData("../assets/models/scoutship/ship1_glass.jpg");
		
		auto meshHandle = graphicsEngine_->createStaticMesh(vertices, indices, colors, normals, textureCoordinates);
		auto textureHandle = graphicsEngine_->createTexture2d(image);
		auto renderableHandle = graphicsEngine_->createRenderable(meshHandle, textureHandle);
		
		auto e = createEntity();
		
		auto collisionShapeHandle = physicsEngine_->createStaticPlane(glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
		
		entities::GraphicsComponent gc;
		gc.renderableHandle = renderableHandle;
		entities::PhysicsComponent pc;
		pc.collisionShapeHandle = collisionShapeHandle;
		assign(e, gc);
		assign(e, pc);
		
		scale(e, 20.0f);
		rotate(e, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		translate(e, glm::vec3(0.0f, -6.0f, 0.0f));
	}
	
	{
		auto model = gameEngine_->importModel("../assets/models/scoutship/scoutship.dae");
		
		auto modelHandle = gameEngine_->loadStaticModel(model);
		
		auto e = createEntity();
		
		std::unique_ptr<HerculesMotionStateListener> motionStateListener = std::make_unique<HerculesMotionStateListener>(e, this);
		auto collisionShapeHandle = physicsEngine_->createBoxShape(glm::vec3(1.0f, 1.0f, 1.0f), std::move(motionStateListener));
		
		entities::GraphicsComponent gc;
		gc.renderableHandle = gameEngine_->createRenderable(modelHandle);
		entities::PhysicsComponent pc;
		pc.collisionShapeHandle = collisionShapeHandle;
		assign(e, gc);
		assign(e, pc);
		
		scale(e, 0.002f);
		translate(e, glm::vec3(1.0f, 0.0f, 1.0f));
		
		//graphicsEngine_->scale(renderableHandle, 0.03f);
		//graphicsEngine_->translate(renderableHandle, 6.0f, -4.0f, 0);
	}
}

Scene::~Scene()
{
}

void Scene::tick(const float32 elapsedTime)
{
	physicsEngine_->tick(elapsedTime);
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

void Scene::assign(const entities::Entity& entity, const entities::GraphicsComponent& component)
{
	logger_->debug( "Assigning graphics component to entity with id: " + std::to_string(entity.getId()) );
	entityComponentSystem_.entities.assign<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()), std::forward<const entities::GraphicsComponent>(component));
}

void Scene::assign(const entities::Entity& entity, const entities::PhysicsComponent& component)
{
	logger_->debug( "Assigning physics component to entity with id: " + std::to_string(entity.getId()) );
	entityComponentSystem_.entities.assign<entities::PhysicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()), std::forward<const entities::PhysicsComponent>(component));
}

void Scene::assign(const entities::Entity& entity, const entities::PositionOrientationComponent& component)
{
	logger_->debug( "Assigning position and orientation component to entity with id: " + std::to_string(entity.getId()) );
	entityComponentSystem_.entities.assign<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()), std::forward<const entities::PositionOrientationComponent>(component));
}
	
void Scene::rotate(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis, const graphics::TransformSpace& relativeTo)
{
	logger_->debug( "Rotating entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
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
	
	graphicsEngine_->rotate(graphicsComponent->renderableHandle, degrees, axis, relativeTo);
}
	
void Scene::rotate(const entities::Entity& entity, const glm::quat& orientation, const graphics::TransformSpace& relativeTo)
{
	logger_->debug( "Rotating entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
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
	
	graphicsEngine_->rotate(graphicsComponent->renderableHandle, orientation, relativeTo);
}

void Scene::rotation(const entities::Entity& entity, const float32 degrees, const glm::vec3& axis)
{
	logger_->debug( "Setting rotation for entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	component->orientation = glm::normalize( glm::angleAxis(glm::radians(degrees), axis) );
	
	graphicsEngine_->rotation(graphicsComponent->renderableHandle, degrees, axis);
}

void Scene::rotation(const entities::Entity& entity, const glm::quat& orientation)
{
	logger_->debug( "Setting rotation for entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	component->orientation = glm::normalize( orientation );
	
	graphicsEngine_->rotation(graphicsComponent->renderableHandle, orientation);
}

void Scene::translate(const entities::Entity& entity, const glm::vec3& translate)
{
	logger_->debug( "Translating entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	component->position += translate;
	
	graphicsEngine_->translate(graphicsComponent->renderableHandle, translate);
}

void Scene::scale(const entities::Entity& entity, const float32 scale)
{
	logger_->debug( "Set scale for entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	component->scale = glm::vec3(scale, scale, scale);
	
	graphicsEngine_->scale(component->renderableHandle, scale);
}

void Scene::scale(const entities::Entity& entity, const glm::vec3& scale)
{
	logger_->debug( "Set scale for entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	component->scale = scale;
	
	graphicsEngine_->scale(component->renderableHandle, scale);
}

void Scene::scale(const entities::Entity& entity, const float32 x, const float32 y, const float32 z)
{
	logger_->debug( "Set scale for entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	component->scale = glm::vec3(x, y, z);
	
	graphicsEngine_->scale(component->renderableHandle, x, y, z);
}

void Scene::lookAt(const entities::Entity& entity, const glm::vec3& lookAt)
{
	logger_->debug( "Set look at for entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	
	const glm::mat4 lookAtMatrix = glm::lookAt(component->position, lookAt, glm::vec3(0.0f, 1.0f, 0.0f));
	component->orientation =  glm::normalize( component->orientation * glm::quat_cast( lookAtMatrix ) );
	
	graphicsEngine_->lookAt(graphicsComponent->renderableHandle, lookAt);
}

void Scene::position(const entities::Entity& entity, const glm::vec3& position)
{
	logger_->debug( "Set position for entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	component->position = position;
	
	graphicsEngine_->position(graphicsComponent->renderableHandle, position);
}

void Scene::position(const entities::Entity& entity, const float32 x, const float32 y, const float32 z)
{
	logger_->debug( "Set position for entity with id: " + std::to_string(entity.getId()) );
	
	auto component = entityComponentSystem_.entities.component<entities::PositionOrientationComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	auto graphicsComponent = entityComponentSystem_.entities.component<entities::GraphicsComponent>(static_cast<entityx::Entity::Id>(entity.getId()));
	component->position = glm::vec3(x, y, z);
	
	graphicsEngine_->position(graphicsComponent->renderableHandle, x, y, z);
}

}
