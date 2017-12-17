#ifndef IPLUGIN_H_
#define IPLUGIN_H_

#include <string>

namespace ice_engine
{

class IPlugin
{
public:
	virtual ~IPlugin()
	{
	}
	;

	virtual std::string getName() const = 0;

};

}

#endif /* IPLUGIN_H_ */
