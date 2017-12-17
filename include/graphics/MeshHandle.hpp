#ifndef MESH_HANDLE_H_
#define MESH_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class MeshHandle : public handles::Handle<MeshHandle>
{
public:
	using handles::Handle<MeshHandle>::Handle;
};

}
}

#endif /* MESH_HANDLE_H_ */
