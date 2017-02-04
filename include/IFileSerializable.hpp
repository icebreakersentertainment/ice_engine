#ifndef IFILESERIALIZABLE_H_
#define IFILESERIALIZABLE_H_

// Required C++ libraries
#include <fstream>

namespace hercules
{

class IFileSerializable
{
public:
	virtual ~IFileSerializable()
	{
	}
	;
	
	virtual void serialize(const std::string& filename) = 0;
	virtual void deserialize(const std::string& filename) = 0;
	
	virtual void serialize(std::ofstream& stream) = 0;
	virtual void deserialize(std::ifstream& stream) = 0;
};

}

#endif /* IFILESERIALIZABLE_H_ */
