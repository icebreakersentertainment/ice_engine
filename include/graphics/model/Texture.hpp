#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

#include "Image.hpp"

namespace ice_engine
{
namespace graphics
{
namespace model
{

struct Texture
{
	Texture() = default;
	Texture(const Texture& other)
	{
		filename = other.filename;
		image = other.image;
	};

	std::string filename;
	Image image;
	//glw::TextureSettings settings;
};

}
}
}

#endif /* TEXTURE_H_ */
