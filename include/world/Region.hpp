#ifndef REGION_H_
#define REGION_H_

#include <string>

#include "Types.hpp"

#include "utilities/Macros.hpp"

namespace hercules
{
namespace world
{

class Region
{
public:
	Region(int32 id = 0, std::string name = std::string(), int32 worldId = 0);
	virtual ~Region();

	void tick(float32 elapsedTime);

	GETSET(int32, id_, Id)
	GET(std::string, name_, Name)
	SET_CONST_REF(std::string, name_, Name)
	GETSET(int32, worldId_, WorldId)
	GETSET(int32, x_, X)
	GETSET(int32, y_, Y)
	
private:
	int32 id_;
	
	std::string name_;
	
	int32 worldId_;
	
	int32 x_;
	
	int32 y_;
};

}
}

#endif /* REGION_H_ */
