#ifndef ILOGGER_H_
#define ILOGGER_H_

#include <string>
#include <cstdio>

#include <boost/format.hpp>

inline boost::format& printFunctionRecursive(boost::format& format)
{
	return format;
}

template <typename Arg, typename ... Args>
inline boost::format& printFunctionRecursive(boost::format& format, const Arg& arg, const Args& ... args)
{
	format % arg;

	return printFunctionRecursive(format, args ...);
}

template <typename ... Args>
inline std::string printFunction(boost::format& format, const Args& ... args)
{
	return boost::str( printFunctionRecursive(format, args ...) );
}

template <typename ... Args>
inline std::string printFunction(const std::string& format, const Args& ... args)
{
	boost::format boostFormat(format);
	return printFunction(boostFormat, args ...);
}

#define LOG_INFO(logger, message, ...) logger->info(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + printFunction(message, ##__VA_ARGS__));
#define LOG_WARN(logger, message, ...) logger->warn(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + printFunction(message, ##__VA_ARGS__));
#define LOG_ERROR(logger, message, ...) logger->error(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + printFunction(message, ##__VA_ARGS__));
#define LOG_FATAL(logger, message, ...) logger->fatal(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + printFunction(message, ##__VA_ARGS__));

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_DEBUG_LOGGING)
	#define LOG_DEBUG(logger, message, ...) logger->debug(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + printFunction(message, ##__VA_ARGS__));
#else
	#define LOG_DEBUG(logger, message, ...)
#endif

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_TRACE_LOGGING)
	#define LOG_TRACE(logger, message, ...) logger->trace(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + printFunction(message, ##__VA_ARGS__));
#else
	#define LOG_TRACE(logger, message, ...)
#endif

namespace ice_engine
{
namespace logger
{

class ILogger
{	
public:
	virtual ~ILogger()
	{
	}
	;
	
	virtual void info(const std::string& message) = 0;
	virtual void debug(const std::string& message) = 0;
	virtual void trace(const std::string& message) = 0;
	virtual void warn(const std::string& message) = 0;
	virtual void error(const std::string& message) = 0;
	virtual void fatal(const std::string& message) = 0;
	
	virtual void info(const std::wstring& message) = 0;
	virtual void debug(const std::wstring& message) = 0;
	virtual void trace(const std::wstring& message) = 0;
	virtual void warn(const std::wstring& message) = 0;
	virtual void error(const std::wstring& message) = 0;
	virtual void fatal(const std::wstring& message) = 0;
};

}

}

#endif /* ILOGGER_H_ */
