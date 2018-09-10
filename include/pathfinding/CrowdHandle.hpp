#ifndef CROWD_HANDLE_H_
#define CROWD_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace pathfinding
{

class CrowdHandle : public handles::Handle<CrowdHandle>
{
public:
	using handles::Handle<CrowdHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::pathfinding::CrowdHandle>
	{
		std::size_t operator()(const ice_engine::pathfinding::CrowdHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* CROWD_HANDLE_H_ */
