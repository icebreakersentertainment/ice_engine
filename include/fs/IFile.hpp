#ifndef IFILE_H_
#define IFILE_H_

#include <string>
#include <iostream>

#include "Types.hpp"

namespace ice_engine
{
namespace fs
{

enum FileFlags : int32
{
	READ 	= 1 << 0,
	WRITE 	= 1 << 1,
	APPEND 	= 1 << 2,
	BINARY 	= 1 << 3
};

class IFile
{
public:
	virtual ~IFile()
	{
	}
	;
	
	virtual bool isOpen() const = 0;
	virtual uint64 size() const = 0;
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
