#ifndef IFILE_H_
#define IFILE_H_

#include <string>
#include <iostream>

#include "Types.hpp"

namespace hercules
{
namespace fs
{

enum FileFlags
{
	READ 	= 1 << 0,
	WRITE 	= 1 << 1,
	APPEND 	= 1 << 2
};

class IFile
{
public:
	virtual ~IFile()
	{
	}
	;
	
	virtual bool isOpen() const = 0;
	virtual bool eof() const = 0;
	virtual void close() = 0;
	
	virtual void write(const char* data) = 0;
	virtual void write(const std::string& file) = 0;
	virtual std::string read(uint32 length = 256) = 0;
	virtual std::string readAll() = 0;
	
	virtual std::istream& getInputStream() = 0;
	virtual std::ostream& getOutputStream() = 0;
};

}
}

#endif /* IFILE_H_ */
