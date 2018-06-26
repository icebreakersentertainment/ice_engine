#ifndef IMODULE_H_
#define IMODULE_H_

#include "Types.hpp"

namespace ice_engine
{

class IModule
{
public:
	virtual ~IModule()
	{
	}
	;
	
	virtual void tick(const float32 delta) = 0;
};

}

#endif /* IMODULE_H_ */
