#include <iostream>
#include <set>
#include <exception>
#include <fstream>
#include <utility>

#include <iomanip>
#include <cctype>

#include <boost/config.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

namespace pod = boost::program_options::detail;

#include "utilities/Properties.hpp"

namespace ice_engine
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

}
}
