#include <fstream>
#include <sstream>
#include <array>

#include "fs/File.hpp"

#include <boost/filesystem.hpp>

#include "exceptions/FileNotFoundException.hpp"
#include "exceptions/InvalidArgumentException.hpp"
#include "exceptions/InvalidOperationException.hpp"

namespace ice_engine
{
namespace fs
{

File::File(const std::string& file, int32 flags) : file_(file), flags_(flags)
{
	const auto path = boost::filesystem::path(file_);
	
	if (flags_ & FileFlags::READ && !boost::filesystem::exists(path))
	{
		throw FileNotFoundException( std::string("Unable to open file - file does not exist: ") + file);
	}

    if (boost::filesystem::is_directory(path))
    {
        throw InvalidArgumentException( std::string("Unable to open file - it's a directory: ") + file);
    }
	
	std::ios_base::openmode openmode = std::ifstream::in;
	
	if (flags_ & FileFlags::BINARY)
	{
		openmode |= std::ifstream::binary;
	}

	if (flags_ & FileFlags::READ)
	{
		inputFileStream_ = std::ifstream(file_, openmode);
	}
	else
	{
		openmode &= ~std::ifstream::in;
		openmode |= std::ofstream::out;
		outputFileStream_ = std::ofstream(file_, openmode);
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

uint64 File::size() const
{
	return static_cast<uint64>(boost::filesystem::file_size(file_));
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
		throw InvalidOperationException( std::string("Unable to write to file - file was opened in READ mode.") );
	}
	
	outputFileStream_ << data;
}

void File::write(const std::string& data)
{
	this->write(data.c_str());
}

std::string File::read(const uint32 length)
{
	if (!(flags_ & FileFlags::READ))
	{
		throw InvalidOperationException( std::string("Unable to read from file - file was not opened in READ mode.") );
	}

	// lazy initialize our buffer
	if (buffer_.empty())
    {
	    buffer_.resize(DEFAULT_BUFFER_SIZE);
    }

    std::stringstream ss;
    int64 count = std::min(length, DEFAULT_BUFFER_SIZE);

	while (count > 0)
    {
        inputFileStream_.read(buffer_.data(), count);
        count -= DEFAULT_BUFFER_SIZE;
        ss << buffer_.data();
    }

	return ss.str();
}

std::string File::readAll()
{
	if (!(flags_ & FileFlags::READ))
	{
		throw InvalidOperationException( std::string("Unable to read from file - file was not opened in READ mode.") );
	}

    std::stringstream buffer;
	buffer << inputFileStream_.rdbuf();
	
	return buffer.str();
}

std::istream& File::getInputStream()
{
	if (!(flags_ & FileFlags::READ))
	{
		throw InvalidOperationException( std::string("Unable to get input stream from file - file was not opened in READ mode.") );
	}
	
	// Get around 'attempting to reference a deleted function' error
	std::istream* is = &inputFileStream_;
	return *is;
}

std::ostream& File::getOutputStream()
{
	if (flags_ & FileFlags::READ)
	{
		throw InvalidOperationException( std::string("Unable to get output stream from file - file was opened in READ mode.") );
	}
	
	// Get around 'attempting to reference a deleted function' error
	std::ostream* os = &outputFileStream_;
	return *os;
}

std::string File::path() const
{
    return file_;
}

}
}
