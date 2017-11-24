#ifndef MATERIAL_HANDLE_H_
#define MATERIAL_HANDLE_H_

#include "handles/Handle.hpp"

namespace hercules
{
namespace graphics
{

class MaterialHandle : public handles::Handle<MaterialHandle>
{
public:
	using handles::Handle<MaterialHandle>::Handle;
};

}
}

#endif /* MATERIAL_HANDLE_H_ */
