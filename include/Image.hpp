#ifndef ICEENGINE_IMAGE_H_
#define ICEENGINE_IMAGE_H_

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

#include "IImage.hpp"

#include "fs/IFile.hpp"

namespace ice_engine
{

class Image : public IImage
{
public:
	Image() = default;

	/**
	 * Will load the provided image data into a proper image.
	 *
	 * @param data The image data to load.
	 * @param hasAlpha Whether the image has an alpha channel or not.
	 */
	Image(std::vector<byte> data, int width, int height, IImage::Format format)
	{
		data_ = std::move(data);
		width_ = width;
		height_ = height;
		format_ = format;
	}

	/**
	 * Will load the provided image data into a proper image.
	 *
	 * @param file The file to load.
	 * @param hasAlpha Whether the image has an alpha channel or not.
	 */
	Image(fs::IFile& file, bool hasAlpha = true) : Image(file.getInputStream(), hasAlpha)
	{
	}

	/**
	 * Will load the provided image data into a proper image.
	 *
	 * @param inputStream Stream from which we will load our image data.
	 * @param hasAlpha Whether the image has an alpha channel or not.
	 */
	Image(std::istream& inputStream, bool hasAlpha = true)
	{
		std::vector<byte> data;

		inputStream.seekg(0, std::ios::end);
		auto filesize = inputStream.tellg();
		inputStream.seekg(0, std::ios::beg);

		data.resize(filesize);

		inputStream.read(reinterpret_cast<char*>(&data[0]), filesize);

		importImage(data, hasAlpha);
	}

	Image(const Image& image)
	{
		data_ = image.data_;
		width_ = image.width_;
		height_ = image.height_;
		format_ = image.format_;
	}

	~Image() override = default;

	const std::vector<byte>& data() const override
	{
		return data_;
	}

	uint32 width() const override
	{
		return width_;
	}

	uint32 height() const override
	{
		return height_;
	}

	int32 format() const override
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

//	std::vector<byte> data_;
//	int width_ = 0;
//	int height_ = 0;
//	IImage::Format format_ = IImage::Format::FORMAT_UNKNOWN;
private:
	//std::vector<char> data_;
	//int width_ = 0;
	//int height_ = 0;
	//IImage::Format format_ = IImage::Format::FORMAT_UNKNOWN;
    std::vector<byte> data_;
    int width_ = 0;
    int height_ = 0;
    IImage::Format format_ = IImage::Format::FORMAT_UNKNOWN;

	void importImage(const std::vector<byte>& data, bool hasAlpha = true);
};

}

#endif /* ICEENGINE_IMAGE_H_ */
