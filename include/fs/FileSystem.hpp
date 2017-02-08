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
	
	virtual bool exists(const std::string& file) const override;
	virtual bool isDirectory(const std::string& file) const override;
	
	virtual void deleteFile(const std::string& file) const override;
	
	virtual void makeDirectory(const std::string& directoryName) const override;
	
	virtual std::string readAll(const std::string& file) const override;
	
	virtual std::unique_ptr<IFile> open(const std::string& file, FileFlags flags) const override;
};

}
}

#endif /* FILESYSTEM_H_ */
