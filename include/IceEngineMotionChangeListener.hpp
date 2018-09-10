#ifndef ICEENGINEMOTIONCHANGELISTENER_H_
#define ICEENGINEMOTIONCHANGELISTENER_H_

#include "physics/IMotionChangeListener.hpp"

#include "ecs/Entity.hpp"

namespace ice_engine
{

class Scene;

class IceEngineMotionChangeListener : public physics::IMotionChangeListener
{
public:
	IceEngineMotionChangeListener(ecs::Entity entity, Scene* scene);
	virtual ~IceEngineMotionChangeListener();
	
	virtual void update(const glm::vec3& position, const glm::quat& orientation) override;

private:
	ecs::Entity entity_;
	Scene* scene_;
};

}

#endif /* ICEENGINEMOTIONCHANGELISTENER_H_ */
