#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include <sstream>

#include <FreeImage.h>

#include "logger/ILogger.hpp"

#include "utilities/Image.hpp"

namespace hercules
{
namespace utilities
{

class ImageLoader
{
public:
	ImageLoader(logger::ILogger* logger) : logger_(logger)
	{
	};
	
	virtual ~ImageLoader()
	{
	};

	Image loadImageData(const std::string filename, bool hasAlpha = true)
	{
		return loadImageData(filename.c_str());
	};

	Image loadImageData(const char* filename, bool hasAlpha = true)
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

		auto retImage = Image();
		
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

			int imageBytesLength = pixelSize * w * h;
			
			// Transfer raw data into a vector
			retImage.data = std::vector<char>(pixels, pixels+imageBytesLength);
			
			retImage.width = w;
			retImage.height = h;
			if (hasAlpha)
			{
				retImage.format = FORMAT_RGBA;
			}
			else
			{
				retImage.format = FORMAT_RGB;
			}
		}
		
		FreeImage_Unload(imageBitmap);

		return std::move(retImage);
	};

private:
	logger::ILogger* logger_;
};

}
}

#endif /* IMAGELOADER_H_ */
