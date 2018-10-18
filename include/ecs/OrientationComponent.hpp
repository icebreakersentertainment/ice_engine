#ifndef ORIENTATIONCOMPONENT_H_
#define ORIENTATIONCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "serialization/glm/Quat.hpp"

namespace ice_engine
{
namespace ecs
{

struct OrientationComponent
{
	OrientationComponent() = default;
	
	OrientationComponent(glm::quat orientation) : orientation(orientation)
	{
	};
	
	static uint8 id()  { return 2; }

	glm::quat orientation;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::OrientationComponent& c, const unsigned int version)
{
	ar & c.orientation;
}

}
}

#endif /* ORIENTATIONCOMPONENT_H_ */
