#ifndef MESH_HANDLE_H_
#define MESH_HANDLE_H_

#include "ResourceHandle.hpp"

namespace hercules
{
namespace graphics
{

class MeshHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const MeshHandle INVALID;
};

}
}

#endif /* MESH_HANDLE_H_ */
