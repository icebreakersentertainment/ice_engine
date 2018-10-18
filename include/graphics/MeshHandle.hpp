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

namespace std
{
	template <>
	struct hash<ice_engine::graphics::MeshHandle>
	{
		std::size_t operator()(const ice_engine::graphics::MeshHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* MESH_HANDLE_H_ */
