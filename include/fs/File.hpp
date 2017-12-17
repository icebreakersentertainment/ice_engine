#ifndef FILE_H_
#define FILE_H_

#include <boost/filesystem/fstream.hpp>

#include "IFile.hpp"

namespace ice_engine
{
namespace fs
{

class File : public IFile
{
public:
	File(const std::string& file, int32 flags);
	virtual ~File();
	
	virtual bool isOpen() const override;
	virtual bool eof() const override;
	virtual void close() override;
	
	virtual void write(const char* data) override;
	virtual void write(const std::string& file) override;
	virtual std::string read(uint32 length = 256) override;
	virtual std::string readAll() override;
	
	virtual std::istream& getInputStream() override;
	virtual std::ostream& getOutputStream() override;

private:
	std::string file_;
	int32 flags_;
	
	std::ifstream inputFileStream_;
	std::ofstream outputFileStream_;
};

}
}

#endif /* FILE_H_ */
