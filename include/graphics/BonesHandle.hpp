#ifndef BONES_HANDLE_H_
#define BONES_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class BonesHandle : public handles::Handle<BonesHandle>
{
public:
	using handles::Handle<BonesHandle>::Handle;
};

}
}

#endif /* BONES_HANDLE_H_ */
