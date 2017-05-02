#ifndef CAMERA_HANDLE_H_
#define CAMERA_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class CameraHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const CameraHandle INVALID;
};

}
}

#endif /* CAMERA_HANDLE_H_ */
