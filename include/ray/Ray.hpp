#ifndef RAY_H_
#define RAY_H_

#include <ostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

namespace hercules
{
namespace ray
{

struct Ray
{
	Ray() = default;

	Ray(const glm::vec3& from, const glm::vec3& to) : from(from), to(to)
	{
	}

	glm::vec3 from;
	glm::vec3 to;
};

}
}

#endif /* RAY_H_ */

