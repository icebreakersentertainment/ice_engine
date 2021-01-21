#ifndef VALIDATION_HPP_
#define VALIDATION_HPP_

#include <boost/type_index.hpp>

#include "detail/Format.hpp"

#include "exceptions/RuntimeException.hpp"

namespace ice_engine
{
namespace detail
{

template<
        typename V,
        typename H,
        typename = typename std::enable_if<std::is_pointer<V>::value, V>::type,
        typename = typename std::enable_if<std::is_base_of<handles::Handle < H>, H> ::value, H>::type
>

inline void checkHandleValidity(V validator, const H handle)
{
    if (!validator->valid(handle))
    {
        const std::string className = boost::typeindex::type_id<H>().pretty_name();
        const std::string message = detail::format("%s handle with id %s is not valid.", className, handle.id());
//        throw InvalidArgumentException(message);
        throw RuntimeException(message);
    }
}

template<
        typename V,
        typename H,
        typename = typename std::enable_if<!std::is_pointer<V>::value, V>::type,
        typename = typename std::enable_if<std::is_base_of<handles::Handle < H>, H> ::value, H>::type
>

inline void checkHandleValidity(const V& validator, const H handle)
{
    if (!validator.valid(handle))
    {
        const std::string className = boost::typeindex::type_id<H>().pretty_name();
        const std::string message = detail::format("%s handle with id %s is not valid.", className, handle.id());
//        throw InvalidArgumentException(message);
        throw RuntimeException(message);
    }
}

}
}

#endif //VALIDATION_HPP_
