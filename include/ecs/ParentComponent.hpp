#ifndef PARENTCOMPONENT_H_
#define PARENTCOMPONENT_H_

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Entity.hpp"

#include "graphics/BonesHandle.hpp"

#include "serialization/glm/Vec3.hpp"

#include "serialization/SplitMember.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct ParentComponent
{
	ParentComponent() = default;

	ParentComponent(Entity entity) : entity(entity)
	{
	};
	
	static uint8 id()  { return 13; }

	Entity entity;
//	graphics::BonesHandle bonesHandle;
//	float32 runningTime = 0.0f;
//	std::vector<glm::mat4> transformations;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::ParentComponent& c, const unsigned int version)
{
	boost::serialization::split_free(ar, c, version);
}

template<class Archive>
void save(Archive& ar, const ice_engine::ecs::ParentComponent& c, const unsigned int version)
{
	ar & c.entity.id().index();
}

template<class Archive>
void load(Archive& ar, ice_engine::ecs::ParentComponent& c, const unsigned int version)
{
	ice_engine::uint32 index;
	ar & index;

	c.entity = ice_engine::ecs::Entity(entityx::Entity::Id(index, 0));
}

}
}

#endif /* PARENTCOMPONENT_H_ */
