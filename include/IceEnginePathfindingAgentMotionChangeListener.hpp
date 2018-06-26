#ifndef ICEENGINEPATHFINDINGMOTIONSTATELISTENER_H_
#define ICEENGINEPATHFINDINGMOTIONSTATELISTENER_H_

#include "pathfinding/IAgentMotionChangeListener.hpp"

#include "Scene.hpp"

namespace ice_engine
{

class IceEnginePathfindingAgentMotionChangeListener : public pathfinding::IAgentMotionChangeListener
{
public:
	IceEnginePathfindingAgentMotionChangeListener(entityx::Entity entity, Scene* scene);
	virtual ~IceEnginePathfindingAgentMotionChangeListener();
	
	virtual void update(const glm::vec3& position) override;

private:
	entityx::Entity entity_;
	Scene* scene_;
};

}

#endif /* ICEENGINEPATHFINDINGMOTIONSTATELISTENER_H_ */
