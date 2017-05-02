#ifndef MODEL_HANDLE_H_
#define MODEL_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class ModelHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const ModelHandle INVALID;
};

}
}

#endif /* MODEL_HANDLE_H_ */
