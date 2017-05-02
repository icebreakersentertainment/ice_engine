#ifndef ANIMATION_HANDLE_H_
#define ANIMATION_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class AnimationHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const AnimationHandle INVALID;
};

}
}

#endif /* ANIMATION_HANDLE_H_ */
