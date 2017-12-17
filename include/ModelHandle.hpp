#ifndef MODEL_HANDLE_H_
#define MODEL_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{

class ModelHandle : public handles::Handle<ModelHandle>
{
public:
	using handles::Handle<ModelHandle>::Handle;
};

}

#endif /* MODEL_HANDLE_H_ */
