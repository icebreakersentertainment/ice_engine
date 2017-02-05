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

bool Properties::toBool(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    std::istringstream is(str);
    
    bool b;
    is >> std::boolalpha >> b;
    return b;
}

std::string Properties::getStringValue(const std::string& name, std::string defaultValue)
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return it->second;
	
	return std::move(defaultValue);
}

int32 Properties::getIntValue(const std::string& name, int32 defaultValue)
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return std::stoi( it->second );
	
	return defaultValue;
	
}

int64 Properties::getLongValue(const std::string& name, int64 defaultValue)
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return std::stol( it->second );
	
	return defaultValue;
}

float32 Properties::getFloatValue(const std::string& name, float32 defaultValue)
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return std::stof( it->second );
	
	return defaultValue;
}

float64 Properties::getDoubleValue(const std::string& name, float64 defaultValue)
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return std::stod( it->second );
	
	return defaultValue;
}

bool Properties::getBoolValue(const std::string& name, bool defaultValue)
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return toBool( it->second );
	
	return defaultValue;
}

}
}
