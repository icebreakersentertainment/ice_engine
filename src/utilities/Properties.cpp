#include <iostream>
#include <string>
#include <set>
#include <sstream>
#include <exception>
#include <fstream>
#include <utility>

#include <iomanip>
#include <algorithm>
#include <cctype>

#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

namespace pod = boost::program_options::detail;

#include "utilities/Properties.hpp"

namespace hercules
{
namespace utilities
{

Properties::Properties()
{
	
}

Properties::Properties(fs::IFile* file)
{
	//parameters
	std::set<std::string> options;
	options.insert("*");
	
	std::istream& configInputStream = file->getInputStream();
	 
	for (pod::config_file_iterator i(configInputStream, options), e ; i != e; ++i)
	{
		parameters_[i->string_key] = i->value[0];
	}
}

Properties::Properties(const std::string& properties)
{
	//parameters
	std::set<std::string> options;
	options.insert("*");
	
	std::istringstream configInputStream = std::istringstream(properties);
	 
	for (pod::config_file_iterator i(configInputStream, options), e ; i != e; ++i)
	{
		parameters_[i->string_key] = i->value[0];
	}
}

Properties::~Properties()
{
}

bool Properties::toBool(std::string str) const
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

std::string Properties::getStringValue(const std::string& name, std::string defaultValue) const
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
	{
		return it->second;
	}
	
	return std::move(defaultValue);
}

std::string Properties::getStringValue(const char* name, const char* defaultValue) const
{
	return getStringValue(std::string(name), std::string(defaultValue));
}

int32 Properties::getIntValue(const std::string& name, const int32 defaultValue) const
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
	{
		return std::stoi( it->second );
	}
	
	return defaultValue;
}

int32 Properties::getIntValue(const char* name, const int32 defaultValue) const
{
	return getIntValue(std::string(name), defaultValue);
}

int64 Properties::getLongValue(const std::string& name, const int64 defaultValue) const
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
	{
		return std::stol( it->second );
	}
	
	return defaultValue;
}

int64 Properties::getLongValue(const char* name, const int64 defaultValue) const
{
	return getLongValue(std::string(name), defaultValue);
}

float32 Properties::getFloatValue(const std::string& name, const float32 defaultValue) const
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return std::stof( it->second );
	
	return defaultValue;
}

float32 Properties::getFloatValue(const char* name, const float32 defaultValue) const
{
	return getFloatValue(std::string(name), defaultValue);
}

float64 Properties::getDoubleValue(const std::string& name, const float64 defaultValue) const
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
	{
		return std::stod( it->second );
	}
	
	return defaultValue;
}

float64 Properties::getDoubleValue(const char* name, const float64 defaultValue) const
{
	return getDoubleValue(std::string(name), defaultValue);
}

bool Properties::getBoolValue(const std::string& name, const bool defaultValue) const
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
	{
		return toBool( it->second );
	}
	
	return defaultValue;
}

bool Properties::getBoolValue(const char* name, const bool defaultValue) const
{
	return getBoolValue(std::string(name), defaultValue);
}

}
}
