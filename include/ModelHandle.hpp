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

namespace std
{
	template <>
	struct hash<ice_engine::ModelHandle>
	{
		std::size_t operator()(const ice_engine::ModelHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* MODEL_HANDLE_H_ */
