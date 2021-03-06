#ifndef RUNTIME_EXCEPTION_H_
#define RUNTIME_EXCEPTION_H_

#include "exceptions/Exception.hpp"

namespace ice_engine
{

class RuntimeException : public Exception
{
	using Exception::Exception;
};

}

#endif /* RUNTIME_EXCEPTION_H_ */
