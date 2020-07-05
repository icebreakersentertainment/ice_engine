#ifndef IFILESYSTEM_H_
#define IFILESYSTEM_H_

#include <string>
#include <vector>
#include <memory>

#include "IFile.hpp"

namespace ice_engine
{
namespace fs
{

class IFileSystem
{
public:
	virtual ~IFileSystem() = default;

	virtual bool exists(const std::string& file) const = 0;
	virtual bool isDirectory(const std::string& file) const = 0;
	virtual std::vector<std::string> list(const std::string& directoryName) const = 0;

	virtual void deleteFile(const std::string& file) const = 0;

	virtual void makeDirectory(const std::string& directoryName) const = 0;

	virtual std::string getBasePath(const std::string& filename) const = 0;
	virtual std::string getDirectorySeperator() const = 0;
	virtual std::string getTempDirectory() const = 0;
	virtual std::string getFilename(const std::string& filename) const = 0;
	virtual std::string getFilenameWithoutExtension(const std::string& filename) const = 0;

	virtual std::string readAll(const std::string& file, const bool isBinary = false) const = 0;

	virtual std::unique_ptr<IFile> open(const std::string& file, int32 flags) const = 0;

	virtual std::string generateTempFilename() const = 0;
};

}
}

#endif /* IFILESYSTEM_H_ */
