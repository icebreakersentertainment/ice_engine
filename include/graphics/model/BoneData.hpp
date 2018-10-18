#ifndef gBONEDATA_H_
#define gBONEDATA_H_

#include <string>
#include <map>
#include <vector>

#include "Types.hpp"

#include "graphics/model/Bone.hpp"

namespace ice_engine
{
namespace graphics
{
namespace model
{

struct BoneData
{
	std::string name;
	std::map< std::string, uint32 > boneIndexMap;
	std::vector< Bone > boneTransform;
};

}
}
}

#endif /* BONEDATA_H_ */
