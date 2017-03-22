#ifndef INVALID_ARGUMENT_EXCEPTION_H_
#define INVALID_ARGUMENT_EXCEPTION_H_

#include "exceptions/Exception.hpp"

namespace hercules
{

class InvalidArgumentException : public Exception
{
	using Exception::Exception;
};

}

#endif /* INVALID_ARGUMENT_EXCEPTION_H_ */
