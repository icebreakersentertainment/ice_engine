#ifndef PHYSICSFACTORY_H_
#define PHYSICSFACTORY_H_

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "IPhysicsEngine.hpp"

namespace physics
{

class PhysicsFactory
{
public:

	static std::unique_ptr<IPhysicsEngine> createPhysicsEngine();

private:
	PhysicsFactory();
	PhysicsFactory(const PhysicsFactory& other);
	virtual ~PhysicsFactory();
};

}

#endif /* PHYSICSFACTORY_H_ */
