#ifndef PHYSICSRAYCAST_H_
#define PHYSICSRAYCAST_H_

#include <ostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "ray/Ray.hpp"

#include "Types.hpp"

namespace hercules
{
namespace physics
{

class Raycast
{
public:
	Raycast() = default;

	Raycast(const ray::Ray& ray) : ray_(ray)
	{
	}

	const ray::Ray& ray() const
	{
		return ray_;
	}

	void setRay(const ray::Ray& ray)
	{
		ray_ = ray;
	}

	const glm::vec3& hitPointWorld() const
	{
		return hitPointWorld_;
	}

	void setHitPointWorld(const glm::vec3& hitPointWorld)
	{
		hitPointWorld_ = hitPointWorld;
	}

	const glm::vec3& hitNormalWorld() const
	{
		return hitNormalWorld_;
	}

	void setHitNormalWorld(const glm::vec3& hitNormalWorld)
	{
		hitNormalWorld_ = hitNormalWorld;
	}

	const RigidBodyObjectHandle& rigidBodyObjectHandle() const
	{
		return rigidBodyObjectHandle_;
	}

	void setRigidBodyObjectHandle(const RigidBodyObjectHandle& rigidBodyObjectHandle)
	{
		rigidBodyObjectHandle_ = rigidBodyObjectHandle;
	}

	const GhostObjectHandle& ghostObjectHandle() const
	{
		return ghostObjectHandle_;
	}

	void setGhostObjectHandle(const GhostObjectHandle& ghostObjectHandle)
	{
		ghostObjectHandle_ = ghostObjectHandle;
	}


protected:
	//~Raycast() = default;

private:
	ray::Ray ray_;
	glm::vec3 hitPointWorld_;
	glm::vec3 hitNormalWorld_;
	RigidBodyObjectHandle rigidBodyObjectHandle_;
	GhostObjectHandle ghostObjectHandle_;
};

}
}

#endif /* PHYSICSRAYCAST_H_ */

