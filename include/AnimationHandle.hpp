#ifndef ANIMATION_HANDLE_H_
#define ANIMATION_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{

class AnimationHandle : public handles::Handle<AnimationHandle>
{
public:
	using handles::Handle<AnimationHandle>::Handle;
};

}

namespace std
{
	template <>
	struct hash<ice_engine::AnimationHandle>
	{
		std::size_t operator()(const ice_engine::AnimationHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* ANIMATION_HANDLE_H_ */
