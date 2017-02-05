#ifndef STRINGUTILITIES_H_
#define STRINGUTILITIES_H_

#include <string>

namespace hercules
{
namespace utilities
{

// Make the functions in an anonymous namespace to avoid multiple definition errors at link time
namespace
{

std::string toString(const std::wstring& ws)
{
	return std::string( ws.begin(), ws.end() );
}

std::wstring toWstring(const std::string& s)
{
	return std::wstring( s.begin(), s.end() );
}

}

}
}

#endif /* STRINGUTILITIES_H_ */
