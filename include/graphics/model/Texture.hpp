#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>

#include "image/Image.hpp"

namespace hercules
{
namespace graphics
{
namespace model
{

struct Texture
{
	std::string filename;
	image::Image image;
	//glw::TextureSettings settings;
};

}
}
}

#endif /* TEXTURE_H_ */
