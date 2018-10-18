#ifndef OBSTACLE_HANDLE_H_
#define OBSTACLE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace pathfinding
{

class ObstacleHandle : public handles::Handle<ObstacleHandle>
{
public:
	using handles::Handle<ObstacleHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::pathfinding::ObstacleHandle>
	{
		std::size_t operator()(const ice_engine::pathfinding::ObstacleHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* OBSTACLE_HANDLE_H_ */
