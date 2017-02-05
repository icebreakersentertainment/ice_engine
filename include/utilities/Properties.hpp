#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <string>
#include <map>

#include "Types.hpp"

#include <fs/IFile.hpp>

namespace hercules
{

namespace utilities
{
	
class Properties
{
public:
	Properties(fs::IFile* file);
	Properties(const std::string& properties);
	virtual ~Properties();
	
	std::string getStringValue(const std::string& name, std::string defaultValue = std::string(""));
	int32 getIntValue(const std::string& name, int32 defaultValue = 0);
	int64 getLongValue(const std::string& name, int64 defaultValue = 0L);
	float32 getFloatValue(const std::string& name, float32 defaultValue = 0.0f);
	float64 getDoubleValue(const std::string& name, float64 defaultValue = 0.0);
	bool getBoolValue(const std::string& name, bool defaultValue = false);

private:
	std::map<std::string, std::string> parameters_;
	
	void initialize();
	
	bool toBool(std::string str);
};

}

}

#endif /* PROPERTIES_H_ */
