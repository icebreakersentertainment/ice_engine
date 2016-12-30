#ifndef GLMUTILITIES_H_
#define GLMUTILITIES_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace utilities
{

const glm::vec3 GLM_UNIT_VECTOR_X = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 GLM_UNIT_VECTOR_Y = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 GLM_UNIT_VECTOR_Z = glm::vec3(0.0f, 0.0f, 1.0f);

bool isVec3InSphere(const glm::vec3& point, const glm::vec3& sphereCenter, glm::detail::float32 radius)
{
	// sqrt((x-x0)^2 + (y-y0)^2 + (z-z0)^2) <=? r
	// (x-x0)^2 + (y-y0)^2 + (z-z0)^2
	
	const glm::vec3 t = sphereCenter - point;
	
	return t.x*t.x + t.y*t.y + t.z*t.z <= radius*radius;
}

}

#endif /* GLMUTILITIES_H_ */
