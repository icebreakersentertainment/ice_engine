#ifndef ILOGGER_H_
#define ILOGGER_H_

#include <string>

#define LOG_INFO(logger, message) logger->info(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + std::string(message));
#define LOG_WARN(logger, message) logger->warn(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + std::string(message));
#define LOG_ERROR(logger, message) logger->error(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + std::string(message));
#define LOG_FATAL(logger, message) logger->fatal(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + std::string(message));

#if defined(DEBUG) || defined(ICEENGINE_ENABLE_DEBUG_LOGGING)
	#define LOG_DEBUG(logger, message) logger->debug(std::string(__FUNCTION__) + " line " + std::to_string(__LINE__) + ": " + std::string(message));
#else
	#define LOG_DEBUG(logger, message)
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
	virtual void warn(const std::string& message) = 0;
	virtual void error(const std::string& message) = 0;
	virtual void fatal(const std::string& message) = 0;
	
	virtual void info(const std::wstring& message) = 0;
	virtual void debug(const std::wstring& message) = 0;
	virtual void warn(const std::wstring& message) = 0;
	virtual void error(const std::wstring& message) = 0;
	virtual void fatal(const std::wstring& message) = 0;
};

}

}

#endif /* ILOGGER_H_ */
