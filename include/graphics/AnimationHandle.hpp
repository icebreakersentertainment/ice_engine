#ifndef ANIMATION_HANDLE_H_
#define ANIMATION_HANDLE_H_

#include "ResourceHandle.hpp"

namespace hercules
{
namespace graphics
{

class AnimationHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const AnimationHandle INVALID;
};

}
}

#endif /* ANIMATION_HANDLE_H_ */
