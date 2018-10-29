#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED

#include <boost/stacktrace/frame.hpp>

#include "exceptions/Stacktrace.hpp"

namespace ice_engine
{

traced stacktrace()
{
	return traced(boost::stacktrace::stacktrace());
}

}
