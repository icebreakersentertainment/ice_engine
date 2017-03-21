#ifndef PHYSICSFACTORY_H_
#define PHYSICSFACTORY_H_

#include <memory>

#include "IPhysicsEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace hercules
{
namespace physics
{

class PhysicsFactory
{
public:

	static std::unique_ptr<IPhysicsEngine> createPhysicsEngine(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	);

private:
	PhysicsFactory();
	PhysicsFactory(const PhysicsFactory& other);
	virtual ~PhysicsFactory();
};

}
}

#endif /* PHYSICSFACTORY_H_ */
