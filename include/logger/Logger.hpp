#ifndef LOGGER_H_
#define LOGGER_H_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace logger = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

using namespace logger::trivial;

#ifdef DEBUG
#define LOG_INFO(x) BOOST_LOG_SEV(cs_logger::Logger::getInstance()->getLogger(), info) << x
#define LOG_DEBUG(x) BOOST_LOG_SEV(cs_logger::Logger::getInstance()->getLogger(), debug) << x
#define LOG_WARN(x) BOOST_LOG_SEV(cs_logger::Logger::getInstance()->getLogger(), warning) << x
#else
#define LOG_INFO(x)
#define LOG_DEBUG(x)
#define LOG_WARN(x)
#endif

#define LOG_ERROR(x) BOOST_LOG_SEV(cs_logger::Logger::getInstance()->getLogger(), error) << x
#define LOG_FATAL(x) BOOST_LOG_SEV(cs_logger::Logger::getInstance()->getLogger(), fatal) << x

namespace cs_logger
{

class Logger
{	
public:
	static Logger* getInstance(const std::string& logFile = std::string("default.log"));

	void logInfo(const std::string& message);
	void logDebug(const std::string& message);
	void logWarn(const std::string& message);
	void logError(const std::string& message);
	void logFatal(const std::string& message);
	
	void logInfo(const std::wstring& message);
	void logDebug(const std::wstring& message);
	void logWarn(const std::wstring& message);
	void logError(const std::wstring& message);
	void logFatal(const std::wstring& message);
	
	// Not super happy with this design - I'd rather not expose the boost log instance in this way - but I
	// don't know how to make it any better...
	src::severity_logger< severity_level >& getLogger();

private:
	Logger(const std::string& fileName);
	Logger(Logger const&);
	Logger& operator=(Logger const&);
	virtual ~Logger();

	void initialize(const std::string& fileName);

	src::severity_logger< severity_level > log_;

	// singleton instance
	static Logger* logger_;
};

}

#endif /* LOGGER_H_ */
