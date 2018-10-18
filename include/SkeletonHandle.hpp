#ifndef SKELETON_HANDLE_H_
#define SKELETON_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{

class SkeletonHandle : public handles::Handle<SkeletonHandle>
{
public:
	using handles::Handle<SkeletonHandle>::Handle;
};

}

namespace std
{
	template <>
	struct hash<ice_engine::SkeletonHandle>
	{
		std::size_t operator()(const ice_engine::SkeletonHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* SKELETON_HANDLE_H_ */
