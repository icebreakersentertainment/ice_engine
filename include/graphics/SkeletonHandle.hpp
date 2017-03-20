#ifndef SKELETON_HANDLE_H_
#define SKELETON_HANDLE_H_

#include "ResourceHandle.hpp"

namespace hercules
{
namespace graphics
{

class SkeletonHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const SkeletonHandle INVALID;
};

}
}

#endif /* SKELETON_HANDLE_H_ */
