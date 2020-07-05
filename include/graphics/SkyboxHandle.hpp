#ifndef SKYBOX_HANDLE_H_
#define SKYBOX_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class SkyboxHandle : public handles::Handle<SkyboxHandle>
{
public:
	using handles::Handle<SkyboxHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::graphics::SkyboxHandle>
	{
		std::size_t operator()(const ice_engine::graphics::SkyboxHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* SKYBOX_HANDLE_H_ */
