#ifndef ILOGGER_H_
#define ILOGGER_H_

#include <string>

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
