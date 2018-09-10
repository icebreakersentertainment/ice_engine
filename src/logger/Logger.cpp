
#include "logger/Logger.hpp"

namespace logger = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace ice_engine
{
namespace logger
{

Logger::Logger(const std::string& fileName)
{
	boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
	
	boost::log::add_file_log( 
		fileName, 
		keywords::auto_flush = true,
		keywords::format = "[%TimeStamp%] - %Severity%: %Message%"
	);

    //boost::log::core::get()->set_filter
    //(
	//	boost::log::trivial::severity >= boost::log::trivial::info
    //);
	
	boost::log::add_common_attributes();
}

Logger::Logger(const Logger& other)
{
	throw std::runtime_error("Cannot copy Logger class.");
}

Logger& Logger::operator=(const Logger& other)
{
	throw std::runtime_error("Cannot reassign Logger class.");
}

Logger::~Logger()
{
}

void Logger::info(const std::string& message)
{
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::info) << message;
}

void Logger::debug(const std::string& message)
{
#if defined(DEBUG) || defined(ICEENGINE_ENABLE_DEBUG_LOGGING)
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::debug) << message;
#endif
}

void Logger::trace(const std::string& message)
{
#if defined(DEBUG) || defined(ICEENGINE_ENABLE_TRACE_LOGGING)
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::trace) << message;
#endif
}

void Logger::warn(const std::string& message)
{
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::warning) << message;
}

void Logger::error(const std::string& message)
{
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::error) << message;
}

void Logger::fatal(const std::string& message)
{
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::fatal) << message;
}

// Wide String versions
void Logger::info(const std::wstring& message)
{
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::info) << message;
}

void Logger::debug(const std::wstring& message)
{
#if defined(DEBUG) || defined(ICEENGINE_ENABLE_DEBUG_LOGGING)
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::debug) << message;
#endif
}

void Logger::trace(const std::wstring& message)
{
#if defined(DEBUG) || defined(ICEENGINE_ENABLE_TRACE_LOGGING)
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::trace) << message;
#endif
}

void Logger::warn(const std::wstring& message)
{
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::warning) << message;
}

void Logger::error(const std::wstring& message)
{
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::error) << message;
}

void Logger::fatal(const std::wstring& message)
{
	BOOST_LOG_SEV(log_, boost::log::trivial::severity_level::fatal) << message;
}

}
}
