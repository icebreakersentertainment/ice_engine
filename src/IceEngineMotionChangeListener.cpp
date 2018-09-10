#include "IceEngineMotionChangeListener.hpp"

#include "Scene.hpp"

namespace ice_engine
{

IceEngineMotionChangeListener::IceEngineMotionChangeListener(ecs::Entity entity, Scene* scene) : entity_(entity), scene_(scene)
{
	
}

IceEngineMotionChangeListener::~IceEngineMotionChangeListener()
{
}

void IceEngineMotionChangeListener::update(const glm::vec3& position, const glm::quat& orientation)
{
	auto pc = entity_.component<ecs:: PositionComponent>();
	auto oc = entity_.component<ecs:: OrientationComponent>();

	pc->position = position;
	oc->orientation = glm::normalize( orientation );

	if (entity_.hasComponent<ecs::DirtyComponent>())
	{
		auto dirtyComponent = entity_.component<ecs::DirtyComponent>();
		dirtyComponent->dirty |= ecs::DirtyFlags::DIRTY_SOURCE_PHYSICS | ecs::DirtyFlags::DIRTY_POSITION | ecs::DirtyFlags::DIRTY_ORIENTATION;
	}
	else
	{
		entity_.assign<ecs::DirtyComponent>(ecs::DirtyFlags::DIRTY_SOURCE_PHYSICS | ecs::DirtyFlags::DIRTY_POSITION | ecs::DirtyFlags::DIRTY_ORIENTATION);
	}
}


}
