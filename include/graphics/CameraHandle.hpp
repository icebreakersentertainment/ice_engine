#ifndef CAMERA_HANDLE_H_
#define CAMERA_HANDLE_H_

#include "ResourceHandle.hpp"

namespace hercules
{
namespace graphics
{

class CameraHandle : public ResourceHandle
{
public:
	using ResourceHandle::ResourceHandle;
	
	static const CameraHandle INVALID;
};

}
}

#endif /* CAMERA_HANDLE_H_ */
