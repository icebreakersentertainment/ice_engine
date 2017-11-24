#ifndef CAMERA_HANDLE_H_
#define CAMERA_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class CameraHandle : public handles::Handle<CameraHandle>
{
public:
	using handles::Handle<CameraHandle>::Handle;
};

}
}

#endif /* CAMERA_HANDLE_H_ */
