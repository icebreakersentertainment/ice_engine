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
	std::vector<char> data((std::istreambuf_iterator<char>(inputStream)), std::istreambuf_iterator<char>());
	
	return std::move(data);
}

}
}

#endif /* IOUTILITIES_H_ */
