#ifndef ILOGGER_H_
#define ILOGGER_H_

#include <string>
#include <cstdio>

// original source: https://stackoverflow.com/a/26221725/780281
template <typename ... Args>
inline std::string printFunction(const std::string& format, const Args ... args)
{
	size_t size = snprintf( nullptr, 0, format.c_str(), args ... ) + 1;

	std::unique_ptr<char[]> buffer(new char[size]);

	snprintf(buffer.get(), size, format.c_str(), args ...);

	return std::string(buffer.get(), buffer.get() + size - 1);
}

#define LOG_INFO(logger, message, ...) logger->info(printFunction(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + message, __VA_ARGS__));
#define LOG_WARN(logger, message, ...) logger->warn(printFunction(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + message, __VA_ARGS__));
#define LOG_ERROR(logger, message, ...) logger->error(printFunction(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + message, __VA_ARGS__));
#define LOG_FATAL(logger, message, ...) logger->fatal(printFunction(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + message, __VA_ARGS__));

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_DEBUG_LOGGING)
	#define LOG_DEBUG(logger, message, ...) logger->debug(printFunction(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + message, __VA_ARGS__));
#else
	#define LOG_DEBUG(logger, message, ...)
#endif

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_TRACE_LOGGING)
	#define LOG_TRACE(logger, message, ...) logger->trace(printFunction(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + message, __VA_ARGS__));
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
