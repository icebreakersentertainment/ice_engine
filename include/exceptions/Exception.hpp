#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
#include <stdexcept>

namespace ice_engine
{

class Exception : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
	
	Exception(const std::string& error) : std::runtime_error(error.c_str())
	{
	};
};

}

#endif /* EXCEPTION_H_ */
