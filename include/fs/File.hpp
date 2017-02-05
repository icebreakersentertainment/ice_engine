#ifndef FILE_H_
#define FILE_H_

#include <boost/filesystem/fstream.hpp>

#include "IFile.hpp"

namespace fs
{

class File : public IFile
{
public:
	File(const std::string& file, FileFlags flags);
	virtual ~File();
	
	virtual bool isOpen() const;
	virtual bool eof() const;
	virtual void close();
	
	virtual void write(const char* data);
	virtual void write(const std::string& file);
	virtual std::string read(glm::detail::uint32 length = 256);
	virtual std::string readAll();
	
	virtual std::istream& getInputStream();
	virtual std::ostream& getOutputStream();

private:
	std::string file_;
	FileFlags flags_;
	
	std::ifstream inputFileStream_;
	std::ofstream outputFileStream_;
};

}

#endif /* FILE_H_ */
