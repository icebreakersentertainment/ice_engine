#ifndef HERCULESMOTIONSTATELISTENER_H_
#define HERCULESMOTIONSTATELISTENER_H_

#include "physics/IMotionStateListener.hpp"

#include "entities/Entity.hpp"
#include "IScene.hpp"

namespace hercules
{

class HerculesMotionStateListener : public physics::IMotionStateListener
{
public:
	HerculesMotionStateListener(entities::Entity entity, IScene* scene);
	virtual ~HerculesMotionStateListener();
	
	virtual void update(const glm::vec3& position, const glm::quat& orientation) override;

private:
	entities::Entity entity_;
	IScene* scene_;
};

}

#endif /* HERCULESMOTIONSTATELISTENER_H_ */
