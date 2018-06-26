#ifndef IAGENTMOTIONCHANGELISTENER_H_
#define IAGENTMOTIONCHANGELISTENER_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace ice_engine
{
namespace pathfinding
{

class IAgentMotionChangeListener
{
public:
	virtual ~IAgentMotionChangeListener()
	{
	}
	;
	
	virtual void update(const glm::vec3& position) = 0;
};

}
}

#endif /* IAGENTMOTIONCHANGELISTENER_H_ */
