#ifndef POLYGON_MESH_HANDLE_H_
#define POLYGON_MESH_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace pathfinding
{

class PolygonMeshHandle : public handles::Handle<PolygonMeshHandle>
{
public:
	using handles::Handle<PolygonMeshHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::pathfinding::PolygonMeshHandle>
	{
		std::size_t operator()(const ice_engine::pathfinding::PolygonMeshHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* POLYGON_MESH_HANDLE_H_ */
