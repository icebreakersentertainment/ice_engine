#ifndef POINT_LIGHT_HANDLE_H_
#define POINT_LIGHT_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class PointLightHandle : public handles::Handle<PointLightHandle>
{
public:
	using handles::Handle<PointLightHandle>::Handle;
};

}
}

#endif /* POINT_LIGHT_HANDLE_H_ */
