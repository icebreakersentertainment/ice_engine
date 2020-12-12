#ifndef INVALIDOPERATIONEXCEPTION_HPP_
#define INVALIDOPERATIONEXCEPTION_HPP_

#include "exceptions/Exception.hpp"

namespace ice_engine
{

struct InvalidOperationException : virtual Exception
{
    using Exception::Exception;
};

}

#endif //INVALIDOPERATIONEXCEPTION_HPP_
