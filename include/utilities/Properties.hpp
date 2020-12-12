#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include <sstream>
#include <algorithm>
#include <string>
#include <unordered_map>

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
		const auto it = parameters_.find(name);

		if (it != parameters_.end()) return it->second;

		return std::move(defaultValue);
	}

	int32 getIntValue(const std::string& name, const int32 defaultValue = 0) const
	{
        const auto it = parameters_.find(name);

		if (it != parameters_.end()) return std::stoi(it->second);

		return defaultValue;
	}

	int64 getLongValue(const std::string& name, const int64 defaultValue = 0L) const
	{
        const auto it = parameters_.find(name);

		if (it != parameters_.end()) return std::stol(it->second);

		return defaultValue;
	}

	float32 getFloatValue(const std::string& name, const float32 defaultValue = 0.0f) const
	{
        const auto it = parameters_.find(name);

		if (it != parameters_.end()) return std::stof(it->second);

		return defaultValue;
	}

	float64 getDoubleValue(const std::string& name, const float64 defaultValue = 0.0) const
	{
        const auto it = parameters_.find(name);

		if (it != parameters_.end()) return std::stod(it->second);

		return defaultValue;
	}

	bool getBoolValue(const std::string& name, const bool defaultValue = false) const
	{
        const auto it = parameters_.find(name);

		if (it != parameters_.end()) return toBool(it->second);

		return defaultValue;
	}

private:
	std::unordered_map<std::string, std::string> parameters_;
};

}

}

#endif /* PROPERTIES_H_ */
