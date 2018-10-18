#ifndef POSITIONCOMPONENT_H_
#define POSITIONCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "serialization/glm/Vec3.hpp"

#include "Types.hpp"

namespace ice_engine
{
namespace ecs
{

struct PositionComponent
{
	PositionComponent() = default;
	
	PositionComponent(glm::vec3 position) : position(position)
	{
	};

	static uint8 id() { return 3; }

	glm::vec3 position;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::PositionComponent& c, const unsigned int version)
{
	ar & c.position;
}

}
}

#endif /* POSITIONCOMPONENT_H_ */
