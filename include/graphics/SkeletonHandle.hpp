#ifndef gSKELETON_HANDLE_H_
#define gSKELETON_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class SkeletonHandle : public handles::Handle<SkeletonHandle>
{
public:
	using handles::Handle<SkeletonHandle>::Handle;
};

}
}

#endif /* SKELETON_HANDLE_H_ */
