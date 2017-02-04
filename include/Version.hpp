#ifndef VERSION_H_
#define VERSION_H_

#include "utilities/Macros.hpp"

namespace hercules
{

class Version
{
public:
	Version(int id = 0, int version = 0);	
	virtual ~Version();
	
	GETSET(int, id_, Id)
	GETSET(int, version_, Version)

private:
	int id_;
	
	int version_;
};

}

#endif /* VERSION_H_ */
