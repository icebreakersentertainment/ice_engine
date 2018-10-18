#ifndef IPHYSICSENGINEFACTORY_H_
#define IPHYSICSENGINEFACTORY_H_

#include <memory>

#include "physics/IPhysicsEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{
namespace physics
{

class IPhysicsEngineFactory
{
public:
	virtual ~IPhysicsEngineFactory()
	{
	}
	;
	
	virtual std::unique_ptr<IPhysicsEngine> create(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	) = 0;
};

}
}

#endif /* IPHYSICSENGINEFACTORY_H_ */
