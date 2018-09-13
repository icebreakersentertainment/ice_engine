#ifndef ICEENGINE_IMAGE_H_
#define ICEENGINE_IMAGE_H_

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include <FreeImage.h>

#include "graphics/IImage.hpp"
#include "physics/IImage.hpp"

namespace ice_engine
{

inline void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message)
{
	std::cerr << message << std::endl;
	//throw std::runtime_error(message);
}

class Image : public graphics::IImage, public physics::IImage
{
public:
	enum Format
	{
		FORMAT_UNKNOWN = -1,
		FORMAT_RGB,
		FORMAT_RGBA
	};

	Image() = default;
	
	/**
	 * Will load the provided image data into a proper image.
	 *
	 * @param data The image data to load.
	 * @param hasAlpha Whether the image has an alpha channel or not.
	 */
	Image(std::vector<char> data, int width, int height, Image::Format format)
	{
		data_ = std::move(data);
		width_ = width;
		height_ = height;
		format_ = format;
	}

	/**
	 * Will load the provided image data into a proper image.
	 *
	 * @param inputStream Stream from which we will load our image data.
	 * @param hasAlpha Whether the image has an alpha channel or not.
	 */
	Image(std::istream& inputStream, bool hasAlpha = true)
	{
		std::vector<char> data;

		inputStream.seekg(0, std::ios::end);
		auto filesize = inputStream.tellg();
		inputStream.seekg(0, std::ios::beg);

		data.resize(filesize);

		inputStream.read(&data[0], filesize);

		importImage(data, hasAlpha);
	}

	Image(const Image& image)
	{
		data_ = image.data_;
		width_ = image.width_;
		height_ = image.height_;
		format_ = image.format_;
	}
	
	virtual ~Image() {}

	virtual const std::vector<char>& data() const override
	{
		return data_;
	}

	virtual uint32 width() const override
	{
		return width_;
	}

	virtual uint32 height() const override
	{
		return height_;
	}

	virtual int32 format() const override
	{
		//return format_;
		switch (format_)
		{
			case Format::FORMAT_RGB:
				return graphics::IImage::Format::FORMAT_RGB;

			case Format::FORMAT_RGBA:
				return graphics::IImage::Format::FORMAT_RGBA;

			case Format::FORMAT_UNKNOWN:
			default:
				return graphics::IImage::Format::FORMAT_UNKNOWN;
		}
	}

	std::vector<char> data_;
	int width_ = 0;
	int height_ = 0;
	Image::Format format_ = Image::Format::FORMAT_UNKNOWN;
private:
	//std::vector<char> data_;
	//int width_ = 0;
	//int height_ = 0;
	//Image::Format format_ = Image::Format::FORMAT_UNKNOWN;

	void importImage(const std::vector<char>& data, bool hasAlpha = true)
	{
		FIMEMORY* stream = nullptr;
		FIBITMAP* bitmap = nullptr;

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
				data_ = std::vector<char>(pixels, pixels+imageBytesLength);

				width_ = w;
				height_ = h;
				if (hasAlpha)
				{
					format_ = FORMAT_RGBA;
				}
				else
				{
					format_ = FORMAT_RGB;
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
	}
};

}

#endif /* ICEENGINE_IMAGE_H_ */