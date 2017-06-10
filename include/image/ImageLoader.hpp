#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include <FreeImage.h>

#include "image/Image.hpp"

namespace hercules
{
namespace image
{

inline void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
{
	std::cerr << message << std::endl;
	//throw std::runtime_error(message);
}

/**
 * Will load the provided image data into a proper image.
 * 
 * @param data The image data to load.
 * @param hasAlpha Whether the image has an alpha channel or not.
 */
inline std::unique_ptr<Image> import(const std::vector<char>& data, bool hasAlpha = true)
{
	FIMEMORY* stream = nullptr;
	FIBITMAP* bitmap = nullptr;
	auto image = std::make_unique<Image>();
	
	try
	{
		FreeImage_SetOutputMessage(FreeImageErrorHandler);
		
		stream = FreeImage_OpenMemory((BYTE*)&data[0], data.size() * sizeof(char));
		FREE_IMAGE_FORMAT format = FreeImage_GetFileTypeFromMemory(stream, 0);
		bitmap =  FreeImage_LoadFromMemory(format, stream);
	
		if (bitmap == nullptr)
		{
			throw std::runtime_error("Unable to load image data");
		}
	
		FIBITMAP* temp = bitmap;
		if ( hasAlpha )
		{
			bitmap = FreeImage_ConvertTo32Bits(bitmap);
		}
		else
		{
			bitmap = FreeImage_ConvertTo24Bits(bitmap);
		}
		
		FreeImage_Unload(temp);
	
		int w = FreeImage_GetWidth(bitmap);
		int h = FreeImage_GetHeight(bitmap);
	
		char* pixels = (char*) FreeImage_GetBits(bitmap);
	
		if ( pixels != nullptr )
		{
			int pixelSize = 3;
			if ( hasAlpha )
			{
				pixelSize = 4;
			}
	
			// FreeImage loads in BGR format, so you need to swap some bytes (Or use GL_BGR)
			for ( int j = 0; j < w * h; j++ )
			{
				char swap = pixels[j * pixelSize + 2];
				pixels[j * pixelSize + 2] = pixels[j * pixelSize + 0];
				pixels[j * pixelSize + 0] = swap;
			}
	
			int imageBytesLength = pixelSize * w * h;
			
			// Transfer raw data into a vector
			image->data = std::vector<char>(pixels, pixels+imageBytesLength);
			
			image->width = w;
			image->height = h;
			if (hasAlpha)
			{
				image->format = FORMAT_RGBA;
			}
			else
			{
				image->format = FORMAT_RGB;
			}
		}
		
		FreeImage_Unload(bitmap);
		FreeImage_CloseMemory(stream);
	}
	catch (const std::exception& e)
	{
		if (bitmap != nullptr)
		{
			FreeImage_Unload(bitmap);
		}
		if (stream != nullptr)
		{
			FreeImage_CloseMemory(stream);
		}
		
		throw e;
	}

	return std::move(image);
}

/**
 * Will load the provided image data into a proper image.
 * 
 * @param inputStream Stream from which we will load our image data.
 * @param hasAlpha Whether the image has an alpha channel or not.
 */
inline std::unique_ptr<Image> import(std::ifstream& inputStream, bool hasAlpha = true)
{
	std::vector<char> data;
	
	inputStream.seekg(0, std::ios::end);
	auto filesize = inputStream.tellg();
	inputStream.seekg(0, std::ios::beg);
	
	data.resize(filesize);
	
	inputStream.read(&data[0], filesize);
	
	return import(data, hasAlpha);
}

}
}

#endif /* IMAGELOADER_H_ */
