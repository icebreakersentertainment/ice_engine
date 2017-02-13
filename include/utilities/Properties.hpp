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
	
	std::string getStringValue(const std::string& name, std::string defaultValue = std::string("")) const;
	std::string getStringValue(const char* name, const char* defaultValue = "") const;
	int32 getIntValue(const std::string& name, const int32 defaultValue = 0) const;
	int32 getIntValue(const char* name, const int32 defaultValue = 0) const;
	int64 getLongValue(const std::string& name, const int64 defaultValue = 0L) const;
	int64 getLongValue(const char* name, const int64 defaultValue = 0L) const;
	float32 getFloatValue(const std::string& name, const float32 defaultValue = 0.0f) const;
	float32 getFloatValue(const char* name, const float32 defaultValue = 0.0f) const;
	float64 getDoubleValue(const std::string& name, const float64 defaultValue = 0.0) const;
	float64 getDoubleValue(const char* name, const float64 defaultValue = 0.0) const;
	bool getBoolValue(const std::string& name, const bool defaultValue = false) const;
	bool getBoolValue(const char* name, const bool defaultValue = false) const;

private:
	std::map<std::string, std::string> parameters_;
	
	bool toBool(std::string str) const;
};

}

}

#endif /* PROPERTIES_H_ */
