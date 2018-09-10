#include <IceEnginePathfindingAgentMotionChangeListener.hpp>

namespace ice_engine
{

IceEnginePathfindingAgentMotionChangeListener::IceEnginePathfindingAgentMotionChangeListener(ecs::Entity entity, Scene* scene) : entity_(entity), scene_(scene)
{
	
}

IceEnginePathfindingAgentMotionChangeListener::~IceEnginePathfindingAgentMotionChangeListener()
{
}

void IceEnginePathfindingAgentMotionChangeListener::update(const glm::vec3& position)
{
	auto pc = entity_.component<ecs:: PositionComponent>();

	pc->position = position;

	if (entity_.hasComponent<ecs::DirtyComponent>())
	{
		auto dirtyComponent = entity_.component<ecs::DirtyComponent>();
		dirtyComponent->dirty |= ecs::DirtyFlags::DIRTY_SOURCE_PATHFINDING | ecs::DirtyFlags::DIRTY_POSITION;
	}
	else
	{
		entity_.assign<ecs::DirtyComponent>(ecs::DirtyFlags::DIRTY_SOURCE_PATHFINDING | ecs::DirtyFlags::DIRTY_POSITION);
	}
}


}
