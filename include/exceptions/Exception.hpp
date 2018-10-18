#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>

//#include "exceptions/Throw.hpp"

namespace ice_engine
{

struct BaseException : virtual boost::exception, virtual std::exception
{
};

struct Exception : virtual BaseException
{
public:
	Exception(const std::string& error = std::string());
	Exception(const std::exception& e);

	const char* what() const noexcept { return message.c_str(); }

protected:
	std::string message;
};

}

#endif /* EXCEPTION_H_ */
