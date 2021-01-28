#ifndef DURATION_HPP_
#define DURATION_HPP_

#include <chrono>

#include "serialization/SplitFree.hpp"

namespace boost
{
namespace serialization
{

template<class Archive, class Rep, class Period>
void serialize(Archive& ar, std::chrono::duration<Rep, Period>& duration, const version_type version)
{
    boost::serialization::split_free(ar, duration, version);
}

template<class Archive, class Rep, class Period>
void save(Archive& ar, const std::chrono::duration<Rep, Period>& duration, const unsigned int version)
{
    ar & duration.count();
}

template<class Archive, class Rep, class Period>
void load(Archive& ar, std::chrono::duration<Rep, Period>& duration, const unsigned int version)
{
    Rep value;

    ar & value;

    duration = std::chrono::duration<Rep, Period>(value);
}

}
}

#endif //DURATION_HPP_
