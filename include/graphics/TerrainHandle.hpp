#ifndef TERRAIN_HANDLE_H_
#define TERRAIN_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class TerrainHandle : public handles::Handle<TerrainHandle>
{
public:
	using handles::Handle<TerrainHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::graphics::TerrainHandle>
	{
		std::size_t operator()(const ice_engine::graphics::TerrainHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* TERRAIN_HANDLE_H_ */
