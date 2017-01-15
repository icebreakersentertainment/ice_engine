#ifndef REGION_H_
#define REGION_H_

#include <string>

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"

#include "utilities/Macros.hpp"

namespace glmd = glm::detail;

namespace game
{
namespace world
{

class Region
{
public:
	Region(int id = 0, std::string name = std::string(), int worldId = 0);
	virtual ~Region();

	void tick(glmd::float32 elapsedTime);

	GETSET(int, id_, Id)
	GET(std::string, name_, Name)
	SET_CONST_REF(std::string, name_, Name)
	GETSET(int, worldId_, WorldId)
	GETSET(int, x_, X)
	GETSET(int, y_, Y)
	
private:
	int id_;
	
	std::string name_;
	
	int worldId_;
	
	int x_;
	
	int y_;
};

}
}

#endif /* REGION_H_ */
