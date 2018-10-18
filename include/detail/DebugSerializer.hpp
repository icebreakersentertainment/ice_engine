#ifndef ICEENGINEDEBUGSERIALIZER_H_
#define ICEENGINEDEBUGSERIALIZER_H_

#include <ostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "handles/HandleVector.hpp"
#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

#define PRINT_TO_STREAM(data, name) << #name ": " << data.name
#define PRINT_DELIMITER() << ", "

namespace glm
{

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& data)
{
//    os << "(glm::vec3)["
//    	PRINT_TO_STREAM(data, x)
//		PRINT_DELIMITER() PRINT_TO_STREAM(data, y)
//		PRINT_DELIMITER() PRINT_TO_STREAM(data, z)
//		<< "]";
//
    os << glm::to_string(data);

    return os;
}

}

#endif /* ICEENGINEDEBUGSERIALIZER_H_ */
