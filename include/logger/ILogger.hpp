#ifndef ILOGGER_H_
#define ILOGGER_H_

#include <string>
#include <cstdio>

#include "detail/Format.hpp"

#define LOG_INFO(logger, message, ...) logger->info(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + ice_engine::detail::format(message, ##__VA_ARGS__));
#define LOG_WARN(logger, message, ...) logger->warn(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + ice_engine::detail::format(message, ##__VA_ARGS__));
#define LOG_ERROR(logger, message, ...) logger->error(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + ice_engine::detail::format(message, ##__VA_ARGS__));
#define LOG_FATAL(logger, message, ...) logger->fatal(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + ice_engine::detail::format(message, ##__VA_ARGS__));

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_DEBUG_LOGGING)
	#define LOG_DEBUG(logger, message, ...) logger->debug(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + ice_engine::detail::format(message, ##__VA_ARGS__));
#else
	#define LOG_DEBUG(logger, message, ...)
#endif

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_TRACE_LOGGING)
	#define LOG_TRACE(logger, message, ...) logger->trace(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + ice_engine::detail::format(message, ##__VA_ARGS__));
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
	virtual ~ILogger() = default;
	
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

static ILogger* gLogger = nullptr;

inline ILogger* globalLogger()
{
    return gLogger;
}

inline void setGlobalLogger(ILogger* globalLogger)
{
    gLogger = globalLogger;
}

}
}

// Global logging
#define GLOBAL_LOG_INFO(message, ...) LOG_INFO(ice_engine::logger::globalLogger(), message, ##__VA_ARGS__);
#define GLOBAL_LOG_WARN(message, ...) LOG_WARN(ice_engine::logger::globalLogger(), message, ##__VA_ARGS__);
#define GLOBAL_LOG_ERROR(message, ...) LOG_ERROR(ice_engine::logger::globalLogger(), message, ##__VA_ARGS__);
#define GLOBAL_LOG_FATAL(message, ...) LOG_FATAL(ice_engine::logger::globalLogger(), message, ##__VA_ARGS__);

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_DEBUG_LOGGING)
#define GLOBAL_LOG_DEBUG(message, ...) LOG_DEBUG(ice_engine::logger::globalLogger(), message, ##__VA_ARGS__);
#else
#define GLOBAL_LOG_DEBUG(message, ...)
#endif

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_TRACE_LOGGING)
#define GLOBAL_LOG_TRACE(message, ...) LOG_TRACE(ice_engine::logger::globalLogger(), message, ##__VA_ARGS__);
#else
#define GLOBAL_LOG_TRACE(message, ...)
#endif

#endif /* ILOGGER_H_ */
