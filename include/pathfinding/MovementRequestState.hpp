#ifndef MOVEMENTREQUESTSTATE_H_
#define MOVEMENTREQUESTSTATE_H_

namespace ice_engine
{
namespace pathfinding
{

enum MovementRequestState
{
	NONE = 0,
	FAILED,
	VALID,
	REQUESTING,
	WAITING_FOR_QUEUE,
	WAITING_FOR_PATH,
	VELOCITY,
};

}
}

#endif /* MOVEMENTREQUESTSTATE_H_ */
