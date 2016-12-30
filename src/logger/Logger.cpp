#include "logger/Logger.hpp"

namespace logger = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

namespace cs_logger
{

Logger::Logger(const std::string& fileName)
{
	initialize(fileName);
}

Logger::Logger(Logger const&)
{
}

Logger::~Logger()
{

}

Logger* Logger::logger_ = nullptr;
Logger* Logger::getInstance(const std::string& logFile)
{
	if ( Logger::logger_ == nullptr )
		Logger::logger_ = new Logger(logFile);

	return Logger::logger_;
}

void Logger::initialize(const std::string& fileName)
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

void Logger::logInfo(const std::string& message)
{
	BOOST_LOG_SEV(log_, info) << message;
}

void Logger::logDebug(const std::string& message)
{
	BOOST_LOG_SEV(log_, debug) << message;
}

void Logger::logWarn(const std::string& message)
{
	BOOST_LOG_SEV(log_, warning) << message;
}

void Logger::logError(const std::string& message)
{
	BOOST_LOG_SEV(log_, error) << message;
}

void Logger::logFatal(const std::string& message)
{
	BOOST_LOG_SEV(log_, fatal) << message;
}

// Wide String versions
void Logger::logInfo(const std::wstring& message)
{
	BOOST_LOG_SEV(log_, info) << message;
}

void Logger::logDebug(const std::wstring& message)
{
	BOOST_LOG_SEV(log_, debug) << message;
}

void Logger::logWarn(const std::wstring& message)
{
	BOOST_LOG_SEV(log_, warning) << message;
}

void Logger::logError(const std::wstring& message)
{
	BOOST_LOG_SEV(log_, error) << message;
}

void Logger::logFatal(const std::wstring& message)
{
	BOOST_LOG_SEV(log_, fatal) << message;
}

src::severity_logger< severity_level >& Logger::getLogger()
{
	return log_;
}

}

