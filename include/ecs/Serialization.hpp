#ifndef ECSSERIALIZATION_H_
#define ECSSERIALIZATION_H_

#include <type_traits>

#include "handles/Handle.hpp"
#include "handles/PointerHandle.hpp"

#include "pathfinding/AgentParams.hpp"
#include "pathfinding/AgentState.hpp"
#include "pathfinding/MovementRequestState.hpp"

#include "serialization/SplitFree.hpp"

namespace boost
{
namespace serialization
{

template<class Archive, class T, typename = typename std::enable_if<std::is_base_of<ice_engine::handles::Handle<T>, T>::value>::type>
void serialize(Archive& ar, T& h, const version_type version)
{
	boost::serialization::split_free(ar, h, version);
}

template<class Archive, class T, typename = typename std::enable_if<std::is_base_of<ice_engine::handles::Handle<T>, T>::value>::type >
void save(Archive& ar, const T& h, const unsigned int version)
{
	ar & h.id();
}

template<class Archive, class T, typename = typename std::enable_if<std::is_base_of<ice_engine::handles::Handle<T>, T>::value>::type >
void load(Archive& ar, T& h, const unsigned int version)
{
	ice_engine::uint64 id;

	ar & id;

	h = T(id);
}

template<class Archive, class T>
typename std::enable_if<std::is_base_of<ice_engine::handles::PointerHandle<T>, T>::value, void>::type
serialize(Archive& ar, T& h, const version_type version)
{
	boost::serialization::split_free(ar, h, version);
}

template<class Archive, class T>
typename std::enable_if<std::is_base_of<ice_engine::handles::PointerHandle<T>, T>::value, void>::type
save(Archive& ar, const T& h, const unsigned int version)
{
	ar & reinterpret_cast<uintptr_t>(h.get());
}

template<class Archive, class T>
typename std::enable_if<std::is_base_of<ice_engine::handles::PointerHandle<T>, T>::value, void>::type
load(Archive& ar, T& h, const unsigned int version)
{
	uintptr_t pointer;

	ar & pointer;

	h = T(reinterpret_cast<void*>(pointer));
}

template<class Archive>
void serialize(Archive& ar, ice_engine::pathfinding::AgentParams& agentParams, const unsigned int version)
{
	ar
		& agentParams.radius
		& agentParams.height
		& agentParams.maxAcceleration
		& agentParams.maxSpeed
		& agentParams.collisionQueryRange
		& agentParams.pathOptimizationRange
		& agentParams.separationWeight
	;
}

}
}

#endif /* ECSSERIALIZATION_H_ */
