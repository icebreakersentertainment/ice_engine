#ifndef IMAGE_H_
#define IMAGE_H_

#include <vector>

#include <GL/glew.h>

namespace ice_engine
{
namespace image
{

enum Format
{
	FORMAT_UNKNOWN = -1,
	FORMAT_RGB,
	FORMAT_RGBA
};

struct Image
{
	Image()
	{
		data = std::vector<char>();
		width = 0;
		height = 0;
		format = Format::FORMAT_UNKNOWN;
	}
	
	Image(const Image& image)
	{
		data = image.data;
		width = image.width;
		height = image.height;
		format = image.format;
	}
	
	std::vector<char> data;
	int width;
	int height;
	Format format;
};

/**
 * Will return the OpenGL compatible format of the given image Format 'format'.
 * 
 * If no known compatible OpenGL format is found, FORMAT_UNKNOWN is returned.
 * 
 * @param format
 * 
 * @return The OpenGL compatible format of the given image Format 'format', or FORMAT_UNKNOWN if no known compatible OpenGL format is found.
 */
inline GLint getOpenGlImageFormat( Format format )
{
	switch (format)
	{
		case Format::FORMAT_RGB:
			return GL_RGB;
		
		case Format::FORMAT_RGBA:
			return GL_RGBA;

		case Format::FORMAT_UNKNOWN:
			return (GLint)FORMAT_UNKNOWN;
	}
	
	return (GLint)FORMAT_UNKNOWN;
}

}
}

#endif /* IMAGE_H_ */
