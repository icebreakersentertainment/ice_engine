#ifndef ICEENGINEMOTIONSTATELISTENER_H_
#define ICEENGINEMOTIONSTATELISTENER_H_

#include "physics/IMotionChangeListener.hpp"

#include "Scene.hpp"

namespace ice_engine
{

class IceEngineMotionChangeListener : public physics::IMotionChangeListener
{
public:
	IceEngineMotionChangeListener(entityx::Entity entity, Scene* scene);
	virtual ~IceEngineMotionChangeListener();
	
	virtual void update(const glm::vec3& position, const glm::quat& orientation) override;

private:
	entityx::Entity entity_;
	Scene* scene_;
};

}

#endif /* ICEENGINEMOTIONSTATELISTENER_H_ */
