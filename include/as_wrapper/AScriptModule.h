#ifndef ASCRIPTMODULE_H_
#define ASCRIPTMODULE_H_

#include <string>
#include <vector>

namespace as_wrapper
{

struct Argument
{
	// TODO: implement
};
	
class AScriptModule
{
public:
	AScriptModule();
	virtual ~AScriptModule();

	void callMethod(std::string name);
	void callMethod(std::string name, std::vector<Argument> arguments);

private:
	void initialize();
	void destroy();
};

}

#endif /* ASCRIPTMODULE_H_ */
