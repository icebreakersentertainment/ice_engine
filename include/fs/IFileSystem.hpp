#ifndef IFILESYSTEM_H_
#define IFILESYSTEM_H_

#include <string>

namespace fs
{

class IFileSystem
{
public:
	virtual ~IFileSystem()
	{
	}
	;
	
	virtual bool exists(const std::string& file) = 0;
	virtual bool isDirectory(const std::string& file) = 0;
	
	virtual void deleteFile(const std::string& file) = 0;
	
	virtual void makeDirectory(const std::string& directoryName) = 0;
	
	virtual std::string readAll(const std::string& file) = 0;
};

}

#endif /* IFILESYSTEM_H_ */
