#ifndef ICEENGINEMOTIONSTATELISTENER_H_
#define ICEENGINEMOTIONSTATELISTENER_H_

#include "physics/IMotionChangeListener.hpp"

#include "entities/Entity.hpp"
#include "IScene.hpp"

namespace ice_engine
{

class IceEngineMotionChangeListener : public physics::IMotionChangeListener
{
public:
	IceEngineMotionChangeListener(entities::Entity entity, IScene* scene);
	virtual ~IceEngineMotionChangeListener();
	
	virtual void update(const glm::vec3& position, const glm::quat& orientation) override;

private:
	entities::Entity entity_;
	IScene* scene_;
};

}

#endif /* ICEENGINEMOTIONSTATELISTENER_H_ */
