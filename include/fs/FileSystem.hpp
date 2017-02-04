#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "IFileSystem.hpp"

namespace fs
{

class FileSystem : public IFileSystem
{
public:
	FileSystem();
	
	virtual bool exists(const std::string& file);
	virtual bool isDirectory(const std::string& file);
	
	virtual void deleteFile(const std::string& file);
	
	virtual void makeDirectory(const std::string& directoryName);
	
	virtual std::string readAll(const std::string& file);
};

}

#endif /* FILESYSTEM_H_ */
