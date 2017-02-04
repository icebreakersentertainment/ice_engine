#ifndef IGAMEENGINE_H_
#define IGAMEENGINE_H_

#include <string>

namespace hercules
{

class IGameEngine
{
public:
	virtual ~IGameEngine()
	{
	}
	;
	
	virtual void run() = 0;
	
	virtual void setBootstrapScript(const std::string& className, const std::string& filename) = 0;
};

}

#endif /* IGAMEENGINE_H_ */
