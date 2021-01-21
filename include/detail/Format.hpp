#ifndef FORMAT_HPP_
#define FORMAT_HPP_

#include <string>

#include <boost/format.hpp>

namespace ice_engine
{
namespace detail
{

inline boost::format& formatFunctionRecursive(boost::format& format)
{
    return format;
}

template<typename Arg, typename ... Args>
inline boost::format& formatFunctionRecursive(boost::format& format, const Arg& arg, const Args& ... args)
{
    format % arg;

    return formatFunctionRecursive(format, args ...);
}

template<typename ... Args>
inline std::string formatFunction(boost::format& format, const Args& ... args)
{
    return boost::str(formatFunctionRecursive(format, args ...));
}

template<typename ... Args>
inline std::string format(const std::string& format, const Args& ... args)
{
    boost::format boostFormat(format);
    return formatFunction(boostFormat, args ...);
}

}
}

#endif //FORMAT_HPP_
