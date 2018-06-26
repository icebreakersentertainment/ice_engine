#include <IceEnginePathfindingAgentMotionChangeListener.hpp>

namespace ice_engine
{

IceEnginePathfindingAgentMotionChangeListener::IceEnginePathfindingAgentMotionChangeListener(entityx::Entity entity, Scene* scene) : entity_(entity), scene_(scene)
{
	
}

IceEnginePathfindingAgentMotionChangeListener::~IceEnginePathfindingAgentMotionChangeListener()
{
}

void IceEnginePathfindingAgentMotionChangeListener::update(const glm::vec3& position)
{
	scene_->position(entity_, position);
}


}
