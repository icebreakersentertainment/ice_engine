#ifndef IMESH_H_
#define IMESH_H_

#include <vector>
#include <string>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Types.hpp"

namespace ice_engine
{
namespace graphics
{

class IMesh
{
public:
	virtual ~IMesh() = default;
	
	virtual const std::string& name() const = 0;
	virtual const std::vector< glm::vec3 >& vertices() const = 0;
	virtual const std::vector< uint32 >& indices() const = 0;
	virtual const std::vector< glm::vec4 >& colors() const = 0;
	virtual const std::vector< glm::vec3 >& normals() const = 0;
	virtual const std::vector< glm::vec2 >& textureCoordinates() const = 0;
};

}
}

#endif /* IMESH_H_ */
