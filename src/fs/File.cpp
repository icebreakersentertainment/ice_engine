#include <fstream>
#include <sstream>

#include "fs/File.hpp"

#include <boost/filesystem.hpp>

namespace fs
{

File::File(const std::string& file, FileFlags flags) : file_(file), flags_(flags)
{
	auto path = boost::filesystem::path(file_);
	
	if (!boost::filesystem::exists(path))
	{
		throw std::runtime_error( std::string("Unable to open file - file does not exist: ") + file);
	}
	
	if (flags_ & FileFlags::READ)
	{
		inputFileStream_ = std::ifstream(file_);
	}
	else
	{
		outputFileStream_ = std::ofstream(file_);
	}
}

File::~File()
{
	this->close();
}

bool File::isOpen() const
{
	if (flags_ & FileFlags::READ)
	{
		return inputFileStream_.is_open();
	}
	else
	{
		return outputFileStream_.is_open();
	}
}

bool File::eof() const
{
	if (flags_ & FileFlags::READ)
	{
		return inputFileStream_.eof();
	}
	else
	{
		return outputFileStream_.eof();
	}
}

void File::close()
{
	if (flags_ & FileFlags::READ)
	{
		inputFileStream_.close();
	}
	else
	{
		outputFileStream_.close();
	}
}
	
void File::write(const char* data)
{
	if (flags_ & FileFlags::READ)
	{
		throw std::runtime_error( std::string("Unable to write to file - file was opened in READ mode.") );
	}
	
	outputFileStream_ << data;
}

void File::write(const std::string& file)
{
	this->write(file.c_str());
}

std::string File::read(glm::detail::uint32 length)
{
	if (!(flags_ & FileFlags::READ))
	{
		throw std::runtime_error( std::string("Unable to write to file - file was not opened in READ mode.") );
	}
	
	std::stringstream buffer;
	buffer << inputFileStream_.rdbuf();
	
	return buffer.str();
}

std::string File::readAll()
{
	if (!(flags_ & FileFlags::READ))
	{
		throw std::runtime_error( std::string("Unable to read from file - file was not opened in READ mode.") );
	}
	
	auto contents = std::string();
	auto line = std::string();

	while ( std::getline(inputFileStream_, line))
	{
		contents += line + '\n';
	}

	return std::move(contents);
}

std::istream& File::getInputStream()
{
	if (!(flags_ & FileFlags::READ))
	{
		throw std::runtime_error( std::string("Unable to get input stream from file - file was not opened in READ mode.") );
	}
	
	// Get around 'attempting to reference a deleted function' error
	std::istream* is = &inputFileStream_;
	return *is;
}

std::ostream& File::getOutputStream()
{
	if (flags_ & FileFlags::READ)
	{
		throw std::runtime_error( std::string("Unable to get output stream from file - file was opened in READ mode.") );
	}
	
	// Get around 'attempting to reference a deleted function' error
	std::ostream* os = &outputFileStream_;
	return *os;
}

}
