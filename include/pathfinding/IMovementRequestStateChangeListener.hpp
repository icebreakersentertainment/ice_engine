#ifndef IMOVEMENTREQUESTSTATECHANGELISTENER_H_
#define IMOVEMENTREQUESTSTATECHANGELISTENER_H_

#include "MovementRequestState.hpp"

namespace ice_engine
{
namespace pathfinding
{

class IMovementRequestStateChangeListener
{
public:
	virtual ~IMovementRequestStateChangeListener()
	{
	}
	;
	
	virtual void update(const MovementRequestState& movementRequestState) = 0;
};

}
}

#endif /* IMOVEMENTREQUESTSTATECHANGELISTENER_H_ */
