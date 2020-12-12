#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <unordered_map>

#include "IFileSystem.hpp"

#include <boost/filesystem.hpp>

namespace ice_engine
{
namespace fs
{

class FileSystem : public IFileSystem
{
public:
    FileSystem(const std::vector<std::string>& baseDirectories = std::vector<std::string>());

    /**
     * Mounts a base directory that will be used when looking for files.
     *
     * You may call this method multiple times to mount several base directories.
     *
     * Note that once you mount a directory, any file that is looked for that does not have its base path within a mounted directory will fail to find the file.
     * @param baseDir
     */
	void mountBaseDirectory(const std::string& baseDir);
	void unmountBaseDirectory(const std::string& baseDir);

    /**
     * Mounts a file that can be accessed by it's mount point.
     *
     * You may call this method multiple times to mount several files.
     * @param baseDir
     */
	void mountFile(const std::string& path, const std::string& mountPoint);
	void unmountFile(const std::string& mountPoint);

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
    std::string getCanonicalPath(const std::string& filename) const override;

    std::string readAll(const std::string& file, const bool isBinary = false) const override;

	std::unique_ptr<IFile> open(const std::string& file, int32 flags) const override;

	std::string generateTempFilename() const override;

private:
    std::vector<std::string> mountedDirectories_;
    std::unordered_map<std::string, std::string> mountedFiles_;

    boost::filesystem::path findPath(const std::string& file) const;
};

}
}

#endif /* FILESYSTEM_H_ */
