#ifndef ICEENGINEDEBUGSERIALIZER_H_
#define ICEENGINEDEBUGSERIALIZER_H_

#include <ostream>

#include <boost/type_index.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "handles/Handle.hpp"
#include "handles/PointerHandle.hpp"
#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

#define PRINT_HEADER_TO_STREAM(class) << "(" << boost::typeindex::type_id<class>().pretty_name() << ")["
#define PRINT_TO_STREAM(data, name) << #name ": " << data.name
#define PRINT_DELIMITER() << ", "
#define PRINT_FOOTER_TO_STREAM() << "]"

// glm
namespace glm
{

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& data)
{
    os << glm::to_string(data);

    return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::quat& data)
{
    os << glm::to_string(data);

    return os;
}

}

// handles
namespace ice_engine
{
namespace handles
{

template <typename H, typename = typename std::enable_if<std::is_base_of<BaseHandle, H>::value, H>::type>
inline std::ostream& operator<<(std::ostream& os, const H& handle)
{
    os  PRINT_HEADER_TO_STREAM(H)
    	PRINT_TO_STREAM(handle, id())
		PRINT_DELIMITER() PRINT_TO_STREAM(handle, index())
		PRINT_DELIMITER() PRINT_TO_STREAM(handle, version())
        PRINT_FOOTER_TO_STREAM()
        ;

    return os;
}

template <typename H, bool unused = false, typename = typename std::enable_if<std::is_base_of<BasePointerHandle, H>::value, H>::type>
inline std::ostream& operator<<(std::ostream& os, const H& handle)
{
    os  PRINT_HEADER_TO_STREAM(H)
    	PRINT_TO_STREAM(handle, get())
        PRINT_FOOTER_TO_STREAM()
        ;

    return os;
}

}
}

#endif /* ICEENGINEDEBUGSERIALIZER_H_ */
