#ifndef STACKTRACE_H_
#define STACKTRACE_H_

#define BOOST_STACKTRACE_GNU_SOURCE_NOT_REQUIRED

#include <boost/stacktrace/stacktrace.hpp>
#include <boost/exception/error_info.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/get_error_info.hpp>

typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;

namespace ice_engine
{

traced stacktrace();

}

#endif /* STACKTRACE_H_ */
