#ifndef MATERIAL_HANDLE_H_
#define MATERIAL_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class MaterialHandle : public handles::Handle
{
public:
	using handles::Handle::Handle;
	
	static const MaterialHandle INVALID;
};

}
}

#endif /* MATERIAL_HANDLE_H_ */
