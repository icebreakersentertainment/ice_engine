#ifndef POINTLIGHTCOMPONENT_H_
#define POINTLIGHTCOMPONENT_H_

#include "graphics/PointLightHandle.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct PointLightComponent
{
	PointLightComponent() = default;
	
	PointLightComponent(graphics::PointLightHandle pointLightHandle) : pointLightHandle(pointLightHandle)
	{
	};
	
	graphics::PointLightHandle pointLightHandle;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::PointLightComponent& c, const unsigned int version)
{
	ar & c.pointLightHandle;
}

}
}

#endif /* POINTLIGHTCOMPONENT_H_ */
