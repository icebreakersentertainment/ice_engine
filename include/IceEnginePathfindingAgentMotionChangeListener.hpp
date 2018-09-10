#ifndef ICEENGINEPATHFINDINGAGENTMOTIONCHANGELISTENER_H_
#define ICEENGINEPATHFINDINGAGENTMOTIONCHANGELISTENER_H_

#include "pathfinding/IAgentMotionChangeListener.hpp"

#include "Scene.hpp"

namespace ice_engine
{

class IceEnginePathfindingAgentMotionChangeListener : public pathfinding::IAgentMotionChangeListener
{
public:
	IceEnginePathfindingAgentMotionChangeListener(ecs::Entity entity, Scene* scene);
	virtual ~IceEnginePathfindingAgentMotionChangeListener();
	
	virtual void update(const glm::vec3& position) override;

private:
	ecs::Entity entity_;
	Scene* scene_;
};

}

#endif /* ICEENGINEPATHFINDINGAGENTMOTIONCHANGELISTENER_H_ */
