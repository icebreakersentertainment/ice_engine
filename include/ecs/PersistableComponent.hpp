#ifndef PERSISTABLECOMPONENT_H_
#define PERSISTABLECOMPONENT_H_

#include "serialization/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct PersistableComponent
{
	PersistableComponent() = default;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::PersistableComponent& c, const unsigned int version)
{

}

}
}

#endif /* PERSISTABLECOMPONENT_H_ */
