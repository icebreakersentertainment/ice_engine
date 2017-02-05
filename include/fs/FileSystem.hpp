#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "IFileSystem.hpp"

namespace hercules
{
namespace fs
{

class FileSystem : public IFileSystem
{
public:
	FileSystem();
	
	virtual bool exists(const std::string& file) const;
	virtual bool isDirectory(const std::string& file) const;
	
	virtual void deleteFile(const std::string& file) const;
	
	virtual void makeDirectory(const std::string& directoryName) const;
	
	virtual std::string readAll(const std::string& file) const;
	
	virtual std::unique_ptr<IFile> open(const std::string& file, FileFlags flags) const;
};

}
}

#endif /* FILESYSTEM_H_ */
