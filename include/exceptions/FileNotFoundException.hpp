#ifndef FILE_NOT_FOUND_EXCEPTION_H_
#define FILE_NOT_FOUND_EXCEPTION_H_

#include "exceptions/Exception.hpp"

namespace ice_engine
{

struct FileNotFoundException : virtual Exception
{
	using Exception::Exception;
};

}

#endif /* FILE_NOT_FOUND_EXCEPTION_H_ */
