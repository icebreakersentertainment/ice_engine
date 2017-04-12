#ifndef HERCULESMOTIONSTATELISTENER_H_
#define HERCULESMOTIONSTATELISTENER_H_

#include "physics/IMotionChangeListener.hpp"

#include "entities/Entity.hpp"
#include "IScene.hpp"

namespace hercules
{

class HerculesMotionChangeListener : public physics::IMotionChangeListener
{
public:
	HerculesMotionChangeListener(entities::Entity entity, IScene* scene);
	virtual ~HerculesMotionChangeListener();
	
	virtual void update(const glm::vec3& position, const glm::quat& orientation) override;

private:
	entities::Entity entity_;
	IScene* scene_;
};

}

#endif /* HERCULESMOTIONSTATELISTENER_H_ */
