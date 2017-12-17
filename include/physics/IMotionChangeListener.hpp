#ifndef IMOTIONSTATELISTENER_H_
#define IMOTIONSTATELISTENER_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace ice_engine
{
namespace physics
{

class IMotionChangeListener
{
public:
	virtual ~IMotionChangeListener()
	{
	}
	;
	
	virtual void update(const glm::vec3& position, const glm::quat& orientation) = 0;
};

}
}

#endif /* IMOTIONSTATELISTENER_H_ */
