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

namespace utilities
{

Properties::Properties(std::string filename) : filename_(filename)
{
	initialize();
}

Properties::~Properties()
{
}

void Properties::initialize()
{	
	std::ifstream config( filename_.c_str() );
	if(!config)
	{
		// TODO: proper error handling
		std::cerr << "error" << std::endl;
		return;
	}

	//parameters
	std::set<std::string> options;
	options.insert("*");
	
	try
	{	  
		for (pod::config_file_iterator i(config, options), e ; i != e; ++i)
		{
			//std::cout << i->string_key << " " << i->value[0] << std::endl;
			parameters_[i->string_key] = i->value[0];
		}
		//std::cout << parameters_["StatLogServer.Path"] << std::endl;
	}
	catch(std::exception& e)	
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}
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

int Properties::getIntValue(const std::string& name, int defaultValue)
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return std::stoi( it->second );
	
	return defaultValue;
	
}

long Properties::getLongValue(const std::string& name, long defaultValue)
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return std::stol( it->second );
	
	return defaultValue;
}

float Properties::getFloatValue(const std::string& name, float defaultValue)
{
	auto it = parameters_.find(name);
	
	if (it != parameters_.end())
		return std::stof( it->second );
	
	return defaultValue;
}

double Properties::getDoubleValue(const std::string& name, double defaultValue)
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
