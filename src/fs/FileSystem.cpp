#include <algorithm>

#include "fs/FileSystem.hpp"
#include "fs/File.hpp"

//#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <exceptions/InvalidArgumentException.hpp>
#include <exceptions/FileNotFoundException.hpp>

namespace ice_engine
{
namespace fs
{

FileSystem::FileSystem(const std::vector<std::string>& baseDirectories)
{
    for (const auto& baseDir : baseDirectories)
    {
        mountBaseDirectory(baseDir);
    }
}

void FileSystem::mountBaseDirectory(const std::string& baseDir)
{
    if (!boost::filesystem::is_directory({baseDir}))
    {
        throw InvalidArgumentException( std::string("Unable to mount base directory '") + baseDir + "' - not a directory.");
    }

    auto canonicalPath = boost::filesystem::weakly_canonical(baseDir);
    canonicalPath.make_preferred();

    if (std::find(mountedDirectories_.cbegin(), mountedDirectories_.cend(), canonicalPath.string()) != mountedDirectories_.cend())
    {
        throw InvalidArgumentException( std::string("Unable to mount base directory '") + baseDir + "' - directory already mounted.");
    }

    mountedDirectories_.push_back(canonicalPath.string());
}

void FileSystem::unmountBaseDirectory(const std::string& baseDir)
{
    const auto it = std::find(mountedDirectories_.cbegin(), mountedDirectories_.cend(), baseDir);

    if (it == mountedDirectories_.cend())
    {
        throw InvalidArgumentException( std::string("Unable to unmount base directory '") + baseDir + "' - directory is not a mounted base directory.");
    }

    mountedDirectories_.erase(it);
}

void FileSystem::mountFile(const std::string& path, const std::string& mountPoint)
{
    if (!boost::filesystem::exists({path}))
    {
        throw FileNotFoundException( std::string("Unable to mount file '") + path + "' - file does not exist.");
    }
    if (boost::filesystem::is_directory({path}))
    {
        throw InvalidArgumentException( std::string("Unable to mount file '") + path + "' - it's a directory.");
    }

    if (mountedFiles_.find(mountPoint) != mountedFiles_.cend())
    {
        throw InvalidArgumentException( std::string("Unable to mount file '") + path + "' - file already mounted to '" + mountPoint + "'.");
    }

    mountedFiles_[mountPoint] = path;
}

void FileSystem::unmountFile(const std::string& mountPoint)
{
    const auto it = mountedFiles_.find(mountPoint);

    if (it == mountedFiles_.cend())
    {
        throw InvalidArgumentException( std::string("Unable to unmount file '") + mountPoint + "' - file is not a mounted file.");
    }

    mountedFiles_.erase(it);
}

boost::filesystem::path FileSystem::findPath(const std::string& file) const
{
    const auto filePath = boost::filesystem::path(file);

//    std::cout << "HIMOM 0 " << filePath << " | " << mountedFiles_.size() << " | " << mountedDirectories_.size() << std::endl;

    // see if this file is any of the mounted files
    for (const auto& kv : mountedFiles_)
    {
        if (kv.first != file) continue;

        const auto path = boost::filesystem::path(kv.second);
//        std::cout << "HIMOM 1a " << path << std::endl;

        auto canonicalPath = boost::filesystem::weakly_canonical(path);
        canonicalPath.make_preferred();
//        std::cout << "HIMOM 3 canonicalPath " << canonicalPath << std::endl;

//        std::cout << "HIMOM 4 " << std::endl;
        return boost::filesystem::path(canonicalPath);
    }

    // see if this file exists in any of the mounted directories
    for (const auto& baseDir : mountedDirectories_)
    {
        const auto baseDirPath = boost::filesystem::path(baseDir);

        //
        if (filePath.is_absolute() && boost::filesystem::exists(filePath))
        {
            auto canonicalPath = boost::filesystem::weakly_canonical(filePath);
            canonicalPath.make_preferred();

            if (std::equal(baseDirPath.begin(), baseDirPath.end(), canonicalPath.begin()))
            {
//                std::cout << "HIMOM 4 " << std::endl;
                return boost::filesystem::path(canonicalPath);
            }
        }
        const auto path = baseDirPath / filePath;
//        std::cout << "HIMOM 1b " << path << std::endl;

        if (boost::filesystem::exists(path))
        {
            // Check the base path
            auto canonicalPath = boost::filesystem::canonical(path, baseDirPath);
//            auto canonicalPath = boost::filesystem::weakly_canonical(path);
            canonicalPath.make_preferred();
//            std::cout << "HIMOM 2 baseDirPath   " << baseDirPath << std::endl;
//            std::cout << "HIMOM 3 canonicalPath " << canonicalPath << std::endl;

            if (std::equal(baseDirPath.begin(), baseDirPath.end(), canonicalPath.begin()))
            {
//                std::cout << "HIMOM 4 " << std::endl;
                return boost::filesystem::path(canonicalPath);
            }
        }
    }

    return boost::filesystem::path();
}

bool FileSystem::exists(const std::string& file) const
{
    const auto path = findPath(file);

    return boost::filesystem::exists(path);
}

bool FileSystem::isDirectory(const std::string& file) const
{
    const auto path = findPath(file);

	return boost::filesystem::is_directory(path);
}

std::vector<std::string> FileSystem::list(const std::string& directoryName) const
{
    const auto path = findPath(directoryName);

	if (!boost::filesystem::exists(path))
	{
		throw FileNotFoundException( std::string("Unable to list files - directory does not exist: ") + directoryName);
	}

	if (!boost::filesystem::is_directory(path))
	{
		throw InvalidArgumentException( std::string("Unable to list files - not a directory: ") + directoryName);
	}

	std::vector<boost::filesystem::path> paths;

	std::copy(boost::filesystem::directory_iterator(path), boost::filesystem::directory_iterator(), std::back_inserter(paths));

	std::sort(paths.begin(), paths.end());

	std::vector<std::string> fileNames;
	std::transform(paths.begin(), paths.end(), std::back_inserter(fileNames), [](const boost::filesystem::path& path) { return path.string(); });

	return fileNames;
}

void FileSystem::deleteFile(const std::string& file) const
{
    const auto path = findPath(file);

	if (!boost::filesystem::exists(path))
	{
		throw FileNotFoundException( std::string("Unable to delete file - file does not exist: ") + file);
	}

	if (boost::filesystem::is_directory(path))
	{
		throw InvalidArgumentException( std::string("Unable to delete file - file is a directory: ") + file);
	}

	boost::filesystem::remove(path);
}

void FileSystem::makeDirectory(const std::string& directoryName) const
{
    const auto parentDirectory = boost::filesystem::path(directoryName).parent_path();
    const auto parentPath = findPath(parentDirectory.string());

    if (!boost::filesystem::exists(parentPath))
    {
        throw FileNotFoundException( std::string("Unable to create directory - parent directory does not exist: ") + directoryName);
    }

    const auto path = boost::filesystem::path(directoryName);

	if (boost::filesystem::exists(path))
	{
		throw InvalidArgumentException( std::string("Unable to create directory - a file or directory by the same name already exists: ") + directoryName);
	}

	boost::filesystem::create_directory(path);
}

std::string FileSystem::getBasePath(const std::string& filename) const
{
	return boost::filesystem::path(filename).parent_path().string();
}

std::string FileSystem::getDirectorySeperator() const
{
	return std::string(1, boost::filesystem::path::preferred_separator);
}

std::string FileSystem::getTempDirectory() const
{
	return boost::filesystem::temp_directory_path().string();
}

std::string FileSystem::getFilename(const std::string& filename) const
{
	return boost::filesystem::path(filename).filename().string();
}

std::string FileSystem::getFilenameWithoutExtension(const std::string& filename) const
{
	return boost::filesystem::path(filename).stem().string();
}

std::string FileSystem::getCanonicalPath(const std::string& filename) const
{
    const auto path = findPath(filename);

    if (!boost::filesystem::exists(path))
    {
        throw FileNotFoundException( std::string("Unable to get canonical path for file - file does not exist: ") + filename);
    }

    return path.string();
}

std::string FileSystem::readAll(const std::string& file, const bool isBinary) const
{
	int32 flags = FileFlags::READ;

	if (isBinary)
	{
		flags |= FileFlags::BINARY;
	}

	auto f = open(file, flags);

	return f->readAll();
}

std::unique_ptr<IFile> FileSystem::open(const std::string& file, int32 flags) const
{
    const auto path = findPath(file); //, flags);

//    std::cout << "HIMOM ABC " << path << " | " << file << std::endl;

    if (!boost::filesystem::exists(path))
    {
        if (flags & FileFlags::READ)
        {
            throw FileNotFoundException(std::string("Unable to open file - file does not exist: ") + file);
        }

        const auto parentDirectory = boost::filesystem::path(file).parent_path();

        if (!parentDirectory.empty())
        {
            const auto parentPath = findPath(parentDirectory.string());

            if (!boost::filesystem::exists(parentPath))
            {
                throw FileNotFoundException( std::string("Unable to open file - parent directory does not exist: ") + file);
            }
        }

        return std::make_unique<File>(file, flags);
    }

	return std::make_unique<File>(path.string(), flags);
}

std::string FileSystem::generateTempFilename() const
{
	const auto tempDirPath = boost::filesystem::temp_directory_path();
    const auto tempFile = tempDirPath / boost::filesystem::unique_path();

	return tempFile.string();
}

}
}
