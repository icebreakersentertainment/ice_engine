#include "HerculesMotionChangeListener.hpp"

namespace hercules
{

HerculesMotionChangeListener::HerculesMotionChangeListener(entities::Entity entity, IScene* scene) : entity_(entity), scene_(scene)
{
	
}

HerculesMotionChangeListener::~HerculesMotionChangeListener()
{
}

void HerculesMotionChangeListener::update(const glm::vec3& position, const glm::quat& orientation)
{
	scene_->rotation(entity_, orientation);
	scene_->position(entity_, position);
}


}
