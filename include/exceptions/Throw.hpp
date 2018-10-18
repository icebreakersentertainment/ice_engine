#ifndef THROW_H_
#define THROW_H_

#include <boost/exception/exception.hpp>
#include <boost/current_function.hpp>

#include "exceptions/Stacktrace.hpp"

namespace ice_engine
{

#define THROW(e) BOOST_THROW_EXCEPTION(boost::enable_error_info(e) << stacktrace())

}

#endif /* THROW_H_ */
