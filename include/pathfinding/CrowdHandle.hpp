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

#endif /* CROWD_HANDLE_H_ */
