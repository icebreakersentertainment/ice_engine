#ifndef ENTITYCOMPONENTSYSTEM_H_
#define ENTITYCOMPONENTSYSTEM_H_

#include <entityx/entityx.h>
#include <entityx/deps/Dependencies.h>

#include "entities/GraphicsComponent.hpp"
#include "entities/RigidBodyObjectComponent.hpp"
#include "entities/GhostObjectComponent.hpp"
#include "entities/PositionOrientationComponent.hpp"

namespace hercules
{
namespace entities
{

class EntityComponentSystem : public entityx::EntityX
{
public:
	EntityComponentSystem() : entityx::EntityX()
	{
		systems.add<entityx::deps::Dependency<GraphicsComponent, PositionOrientationComponent>>();
		systems.add<entityx::deps::Dependency<RigidBodyObjectComponent, PositionOrientationComponent>>();
		systems.add<entityx::deps::Dependency<GhostObjectComponent, PositionOrientationComponent>>();
		systems.configure();
	}
	;
	
	virtual ~EntityComponentSystem()
	{
	}
	;
};

}
}

#endif /* ENTITYCOMPONENTSYSTEM_H_ */
