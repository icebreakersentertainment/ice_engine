#ifndef SKELETON_HANDLE_H_
#define SKELETON_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class SkeletonHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const SkeletonHandle INVALID;
};

}
}

#endif /* SKELETON_HANDLE_H_ */
