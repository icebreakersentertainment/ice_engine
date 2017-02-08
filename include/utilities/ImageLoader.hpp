#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include <sstream>

#include <FreeImage.h>

#include <GL/glew.h>

#include "logger/ILogger.hpp"

namespace hercules
{
namespace utilities
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

class ImageLoader
{
public:
	ImageLoader(logger::ILogger* logger) : logger_(logger)
	{
	};
	
	virtual ~ImageLoader()
	{
	};

	std::unique_ptr<Image> loadImageData(const std::string filename, bool hasAlpha = true)
	{
		return loadImageData(filename.c_str());
	};

	std::unique_ptr<Image> loadImageData(const char* filename, bool hasAlpha = true)
	{
		logger_->debug( "Loading image." );
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
		FIBITMAP* imageBitmap = FreeImage_Load(format, filename);

		if (imageBitmap == nullptr)
		{
			std::stringstream ss;
			ss << "Unable to load image data from file '" << filename << "'.";
			throw std::runtime_error(ss.str());
		}
		
		logger_->debug( "image loaded." );

		FIBITMAP* temp = imageBitmap;
		if ( hasAlpha )
		{
			imageBitmap = FreeImage_ConvertTo32Bits(imageBitmap);
		}
		else
		{
			imageBitmap = FreeImage_ConvertTo24Bits(imageBitmap);
		}
		
		FreeImage_Unload(temp);

		int w = FreeImage_GetWidth(imageBitmap);
		int h = FreeImage_GetHeight(imageBitmap);
		
		std::stringstream msg;
		msg << "The size of the image '" << filename << "' is: " << w << "*" << h;
		logger_->debug( msg.str() );

		char* pixels = (char*) FreeImage_GetBits(imageBitmap);

		auto retImage = std::unique_ptr<Image>();
		
		if ( pixels != nullptr )
		{
			int pixelSize = 3;
			if ( hasAlpha )
				pixelSize = 4;

			// FreeImage loads in BGR format, so you need to swap some bytes (Or use GL_BGR)
			for ( int j = 0; j < w * h; j++ )
			{
				char swap = pixels[j * pixelSize + 2];
				pixels[j * pixelSize + 2] = pixels[j * pixelSize + 0];
				pixels[j * pixelSize + 0] = swap;
			}

			retImage = std::unique_ptr<Image>(new Image());
			
			int imageBytesLength = pixelSize * w * h;
			
			// Transfer raw data into a vector
			retImage->data = std::vector<char>(pixels, pixels+imageBytesLength);
			
			retImage->width = w;
			retImage->height = h;
			if (hasAlpha)
				retImage->format = FORMAT_RGBA;
			else
				retImage->format = FORMAT_RGB;
		}
		
		FreeImage_Unload(imageBitmap);

		return retImage;
	};

private:
	logger::ILogger* logger_;
};

}

}

#endif /* IMAGELOADER_H_ */
