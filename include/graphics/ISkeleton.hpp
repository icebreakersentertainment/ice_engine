#ifndef ISKELETON_H_
#define ISKELETON_H_

#include <map>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{

class ISkeleton
{
public:
	virtual ~ISkeleton() = default;
	
	virtual const std::string& name() const = 0;
	virtual const std::vector<glm::ivec4>& boneIds() const = 0;
	virtual const std::vector<glm::vec4>& boneWeights() const = 0;
};

}
}

#endif /* ISKELETON_H_ */
