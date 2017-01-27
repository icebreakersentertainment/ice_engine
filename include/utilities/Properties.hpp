#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <string>
#include <map>

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
