#include "HerculesMotionStateListener.hpp"

namespace hercules
{

HerculesMotionStateListener::HerculesMotionStateListener(entities::Entity entity, IScene* scene) : entity_(entity), scene_(scene)
{
	
}

HerculesMotionStateListener::~HerculesMotionStateListener()
{
}

void HerculesMotionStateListener::update(const glm::vec3& position, const glm::quat& orientation)
{
	scene_->rotation(entity_, orientation);
	scene_->position(entity_, position);
}


}
