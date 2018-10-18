#ifndef CHILDRENCOMPONENT_H_
#define CHILDRENCOMPONENT_H_

#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Entity.hpp"

#include "graphics/BonesHandle.hpp"

#include "serialization/std/Vector.hpp"

#include "serialization/SplitMember.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct ChildrenComponent
{
	ChildrenComponent() = default;

	ChildrenComponent(std::vector<Entity> children) : children(children)
	{
	};
	
	static uint8 id()  { return 14; }

	std::vector<Entity> children;
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
void serialize(Archive& ar, ice_engine::ecs::ChildrenComponent& c, const unsigned int version)
{
	boost::serialization::split_free(ar, c, version);
}

template<class Archive>
void save(Archive& ar, const ice_engine::ecs::ChildrenComponent& c, const unsigned int version)
{
	ar & c.children.size();

	for (const auto& entity : c.children)
	{
		ar & entity.id().index();
	}
}

template<class Archive>
void load(Archive& ar, ice_engine::ecs::ChildrenComponent& c, const unsigned int version)
{
	size_t size;

	for (int i=0; i < size; ++i)
	{
		ice_engine::uint32 index;
		ar & index;

		c.children.emplace_back(entityx::Entity::Id(index, 0));
	}
}

}
}

#endif /* CHILDRENCOMPONENT_H_ */
