#ifndef BONEDATA_H_
#define BONEDATA_H_

#include <string>
#include <map>
#include <vector>

#include "Types.hpp"

#include "Bone.hpp"

namespace hercules
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
