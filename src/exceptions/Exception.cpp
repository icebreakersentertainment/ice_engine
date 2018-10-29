#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED

#include <boost/stacktrace/frame.hpp>
#include <boost/exception/exception.hpp>
#include <boost/current_function.hpp>

#include "exceptions/Exception.hpp"
#include "exceptions/Stacktrace.hpp"

namespace ice_engine
{

Exception::Exception(const std::string& error) : message(error)
{
	*this << stacktrace();
}

Exception::Exception(const std::exception& e) : Exception(e.what())
{
}

}
