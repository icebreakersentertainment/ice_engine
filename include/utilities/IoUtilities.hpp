#ifndef IOUTILITIES_H_
#define IOUTILITIES_H_

#include <ios>
#include <vector>

namespace ice_engine
{
namespace utilities
{

inline std::vector<char> readAllBytes(std::istream& inputStream)
{
	std::vector<char> data;
	
	inputStream.seekg(0, std::ios::end);
	auto filesize = inputStream.tellg();
	inputStream.seekg(0, std::ios::beg);
	
	data.resize(filesize);
	
	inputStream.read(&data[0], filesize);
	
	return std::move(data);
}

}
}

#endif /* IOUTILITIES_H_ */
