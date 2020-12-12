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
	~File() override;
	
	bool isOpen() const override;
	uint64 size() const override;
	bool eof() const override;

    std::string path() const override;

    void close() override;
	
	void write(const char* data) override;
	void write(const std::string& data) override;
	std::string read(const uint32 length = 256) override;
	std::string readAll() override;
	
	std::istream& getInputStream() override;
	std::ostream& getOutputStream() override;

private:
	std::string file_;
	int32 flags_;

	static constexpr uint32 DEFAULT_BUFFER_SIZE = 1024;
    std::vector<char> buffer_;
	
	std::ifstream inputFileStream_;
	std::ofstream outputFileStream_;
};

}
}

#endif /* FILE_H_ */
