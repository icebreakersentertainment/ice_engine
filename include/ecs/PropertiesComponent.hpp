#ifndef PROPERTIESCOMPONENT_HPP_
#define PROPERTIESCOMPONENT_HPP_

#include <unordered_map>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Entity.hpp"

#include "graphics/BonesHandle.hpp"

#include "serialization/std/UnorderedMap.hpp"

#include "serialization/SplitMember.hpp"

#include "ecs/Serialization.hpp"

namespace ice_engine
{
namespace ecs
{

struct PropertiesComponent
{
    PropertiesComponent() = default;

    PropertiesComponent(std::unordered_map<std::string, std::string> properties) : properties(properties)
    {
    };

    static uint8 id()  { return 17; }

    std::unordered_map<std::string, std::string> properties;
};

}
}

namespace boost
{
namespace serialization
{

template<class Archive>
void serialize(Archive& ar, ice_engine::ecs::PropertiesComponent& c, const unsigned int version)
{
    boost::serialization::split_free(ar, c, version);
}

template<class Archive>
void save(Archive& ar, const ice_engine::ecs::PropertiesComponent& c, const unsigned int version)
{
    ar & c.properties;
}

template<class Archive>
void load(Archive& ar, ice_engine::ecs::PropertiesComponent& c, const unsigned int version)
{
    ar & c.properties;
}

}
}

#endif //PROPERTIESCOMPONENT_HPP_
