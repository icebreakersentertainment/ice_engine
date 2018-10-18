#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <sstream>
#include <algorithm>
#include <string>
#include <map>

#include "Types.hpp"

#include <fs/IFile.hpp>

namespace ice_engine
{

namespace utilities
{

namespace
{
bool toBool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);

    bool b;
    is >> std::boolalpha >> b;
    return b;
}
}

class Properties
{
public:
	Properties() = default;
	Properties(fs::IFile* file);
	Properties(const std::string& properties);
	~Properties() = default;
	
	std::string getStringValue(const std::string& name, std::string defaultValue = std::string()) const
	{
		auto it = parameters_.find(name);

		if (it != parameters_.end())
		{
			return it->second;
		}

		return std::move(defaultValue);
	}

	std::string getStringValue(const char* name, const char* defaultValue = "") const
	{
		return getStringValue(std::string(name), std::string(defaultValue));
	}

	int32 getIntValue(const std::string& name, const int32 defaultValue = 0) const
	{
		auto it = parameters_.find(name);

		if (it != parameters_.end())
		{
			return std::stoi( it->second );
		}

		return defaultValue;
	}

	int32 getIntValue(const char* name, const int32 defaultValue = 0) const
	{
		return getIntValue(std::string(name), defaultValue);
	}

	int64 getLongValue(const std::string& name, const int64 defaultValue = 0L) const
	{
		auto it = parameters_.find(name);

		if (it != parameters_.end())
		{
			return std::stol( it->second );
		}

		return defaultValue;
	}

	int64 getLongValue(const char* name, const int64 defaultValue = 0L) const
	{
		return getLongValue(std::string(name), defaultValue);
	}

	float32 getFloatValue(const std::string& name, const float32 defaultValue = 0.0f) const
	{
		auto it = parameters_.find(name);

		if (it != parameters_.end())
			return std::stof( it->second );

		return defaultValue;
	}

	float32 getFloatValue(const char* name, const float32 defaultValue = 0.0f) const
	{
		return getFloatValue(std::string(name), defaultValue);
	}

	float64 getDoubleValue(const std::string& name, const float64 defaultValue = 0.0) const
	{
		auto it = parameters_.find(name);

		if (it != parameters_.end())
		{
			return std::stod( it->second );
		}

		return defaultValue;
	}

	float64 getDoubleValue(const char* name, const float64 defaultValue = 0.0) const
	{
		return getDoubleValue(std::string(name), defaultValue);
	}

	bool getBoolValue(const std::string& name, const bool defaultValue = false) const
	{
		auto it = parameters_.find(name);

		if (it != parameters_.end())
		{
			return toBool( it->second );
		}

		return defaultValue;
	}

	bool getBoolValue(const char* name, const bool defaultValue = false) const
	{
		return getBoolValue(std::string(name), defaultValue);
	}

private:
	std::map<std::string, std::string> parameters_;
};

}

}

#endif /* PROPERTIES_H_ */
