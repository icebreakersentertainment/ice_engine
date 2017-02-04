#include "world/Region.hpp"

namespace hercules
{
namespace world
{
	
Region::Region(int id, std::string name, int worldId) : id_(id), name_(name), worldId_(worldId)
{
}

Region::~Region()
{
}

void Region::tick(glmd::float32 elapsedTime)
{
	// Not sure I need to call tick for locales...
	//for (auto& l : locales_)
	//{		
	//	l.tick(elapsedTime);
	//}
}

}
}
