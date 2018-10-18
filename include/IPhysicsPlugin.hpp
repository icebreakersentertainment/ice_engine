#ifndef IPHYSICSPLUGIN_H_
#define IPHYSICSPLUGIN_H_

#include <memory>

#include "physics/IPhysicsEngineFactory.hpp"

#include "IPlugin.hpp"

namespace ice_engine
{

class IPhysicsPlugin : public virtual IPlugin
{
public:
	virtual ~IPhysicsPlugin()
	{
	}
	;

	virtual std::unique_ptr<physics::IPhysicsEngineFactory> createFactory() const = 0;

};

}

#endif /* IPHYSICSPLUGIN_H_ */
