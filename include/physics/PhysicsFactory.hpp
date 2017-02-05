#ifndef PHYSICSFACTORY_H_
#define PHYSICSFACTORY_H_

#include <memory>

#include "IPhysicsEngine.hpp"

namespace hercules
{
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
}

#endif /* PHYSICSFACTORY_H_ */
