#include <algorithm>

#include "fs/FileSystem.hpp"
#include "fs/File.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace ice_engine
{
namespace fs
{

bool FileSystem::exists(const std::string& file) const
{
	auto path = boost::filesystem::path(file);

	return boost::filesystem::exists(path);
}

bool FileSystem::isDirectory(const std::string& file) const
{
	auto path = boost::filesystem::path(file);

	return boost::filesystem::is_directory(path);
}

std::vector<std::string> FileSystem::list(const std::string& directoryName) const
{
	auto path = boost::filesystem::path(directoryName);

	if (!boost::filesystem::exists(path))
	{
		throw std::runtime_error( std::string("Unable to list files - directory does not exist: ") + directoryName);
	}

	if (!boost::filesystem::is_directory(path))
	{
		throw std::runtime_error( std::string("Unable to list files - not a directory: ") + directoryName);
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
	auto path = boost::filesystem::path(file);

	if (!boost::filesystem::exists(path))
	{
		throw std::runtime_error( std::string("Unable to delete file - file does not exist: ") + file);
	}

	if (boost::filesystem::is_directory(path))
	{
		throw std::runtime_error( std::string("Unable to delete file - file is a directory: ") + file);
	}

	boost::filesystem::remove(path);
}

void FileSystem::makeDirectory(const std::string& directoryName) const
{
	auto path = boost::filesystem::path(directoryName);

	if (boost::filesystem::is_directory(path))
	{
		throw std::runtime_error( std::string("Unable to create directory - directory already exists: ") + directoryName);
	}

	if (boost::filesystem::exists(path))
	{
		throw std::runtime_error( std::string("Unable to create directory - a file by the same name already exists: ") + directoryName);
	}

	boost::filesystem::create_directory(path);
}

std::string FileSystem::getBasePath(const std::string& filename) const
{
	auto path = boost::filesystem::path(filename);
	return path.parent_path().string();
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
	auto path = boost::filesystem::path(filename);
	return path.filename().string();
}

std::string FileSystem::getFilenameWithoutExtension(const std::string& filename) const
{
	auto path = boost::filesystem::path(filename);
	return path.stem().string();
}

std::string FileSystem::readAll(const std::string& file, const bool isBinary) const
{
	int32 flags = FileFlags::READ;

	if (isBinary)
	{
		flags |= FileFlags::BINARY;
	}

	auto f = this->open(file, flags);

	return f->readAll();
}

std::unique_ptr<IFile> FileSystem::open(const std::string& file, int32 flags) const
{
	return std::make_unique<File>( file, flags );
}

std::string FileSystem::generateTempFilename() const
{
	auto tempDirPath = boost::filesystem::temp_directory_path();
	auto tempFile = tempDirPath / boost::filesystem::unique_path();

	return tempFile.string();
}

}
}
