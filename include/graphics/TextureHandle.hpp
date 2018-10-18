#ifndef TEXTURE_HANDLE_H_
#define TEXTURE_HANDLE_H_

#include "handles/Handle.hpp"

namespace ice_engine
{
namespace graphics
{

class TextureHandle : public handles::Handle<TextureHandle>
{
public:
	using handles::Handle<TextureHandle>::Handle;
};

}
}

namespace std
{
	template <>
	struct hash<ice_engine::graphics::TextureHandle>
	{
		std::size_t operator()(const ice_engine::graphics::TextureHandle& k) const noexcept
		{
			return hash<ice_engine::uint64>{}(k.id());
		}
	};
}

#endif /* TEXTURE_HANDLE_H_ */
