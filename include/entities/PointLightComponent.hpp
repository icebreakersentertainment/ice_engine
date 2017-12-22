#ifndef POINTLIGHTCOMPONENT_H_
#define POINTLIGHTCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "graphics/PointLightHandle.hpp"

namespace hercules
{
namespace entities
{

struct PointLightComponent
{
	PointLightComponent() = default;
	
	PointLightComponent(glm::vec3 position, graphics::PointLightHandle pointLightHandle) : position(position), pointLightHandle(pointLightHandle)
	{
	};
	
	glm::vec3 position;
	graphics::PointLightHandle pointLightHandle;
};

}
}

#endif /* POINTLIGHTCOMPONENT_H_ */
