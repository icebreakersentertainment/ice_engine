#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>

namespace hercules
{

class Exception : public std::exception
{
public:
	using std::exception::exception;
	
	Exception(const std::string& error) : std::exception(error.c_str())
	{
	};
};

}

#endif /* EXCEPTION_H_ */
