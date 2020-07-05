#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "IFileSystem.hpp"

namespace ice_engine
{
namespace fs
{

class FileSystem : public IFileSystem
{
public:
	bool exists(const std::string& file) const override;
	bool isDirectory(const std::string& file) const override;
	std::vector<std::string> list(const std::string& directoryName) const override;

	void deleteFile(const std::string& file) const override;

	void makeDirectory(const std::string& directoryName) const override;

	std::string getBasePath(const std::string& filename) const override;
	std::string getDirectorySeperator() const override;
	std::string getTempDirectory() const override;
	std::string getFilename(const std::string& filename) const override;
	std::string getFilenameWithoutExtension(const std::string& filename) const override;

	std::string readAll(const std::string& file, const bool isBinary = false) const override;

	std::unique_ptr<IFile> open(const std::string& file, int32 flags) const override;

	std::string generateTempFilename() const override;
};

}
}

#endif /* FILESYSTEM_H_ */
