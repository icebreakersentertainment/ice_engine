#include "utilities/fs/FileSystem.hpp"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace utilities
{

namespace fs
{

FileSystem::FileSystem()
{
}
	
bool FileSystem::exists(const std::string& file)
{
	auto path = boost::filesystem::path(file);
	
	return boost::filesystem::exists(path);
}

bool FileSystem::isDirectory(const std::string& file)
{
	auto path = boost::filesystem::path(file);
	
	return boost::filesystem::is_directory(path);
}

void FileSystem::deleteFile(const std::string& file)
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

void FileSystem::makeDirectory(const std::string& directoryName)
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

std::string FileSystem::readAll(const std::string& file)
{
	auto path = boost::filesystem::path(file);
	
	if (!boost::filesystem::exists(path))
	{
		throw std::runtime_error( std::string("Unable to read file - file does not exist: ") + file);
	}
	
	boost::filesystem::ifstream f(path);
	
	auto contents = std::string();
	auto line = std::string();

	while ( getline(f, line))
	{
		contents += line + '\n';
	}

	f.close();
	
	return contents;
}

}

}
