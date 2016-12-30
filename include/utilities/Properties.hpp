#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <string.h>
#include <map>

#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

namespace pod = boost::program_options::detail;

namespace utilities
{
	
class Properties
{
public:
	Properties(std::string filename);
	virtual ~Properties();
	
	std::string getStringValue(const std::string& name, std::string defaultValue = std::string(""));
	int getIntValue(const std::string& name, int defaultValue = 0);
	long getLongValue(const std::string& name, long defaultValue = 0L);
	float getFloatValue(const std::string& name, float defaultValue = 0.0f);
	double getDoubleValue(const std::string& name, double defaultValue = 0.0);
	bool getBoolValue(const std::string& name, bool defaultValue = false);

private:
	std::string filename_;
	std::map<std::string, std::string> parameters_;
	
	void initialize();
	
	bool toBool(std::string str);
};

}

#endif /* PROPERTIES_H_ */
