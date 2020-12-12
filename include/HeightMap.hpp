#ifndef HEIGHTMAP_H_
#define HEIGHTMAP_H_

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include "graphics/IHeightMap.hpp"

#include "Image.hpp"

namespace ice_engine
{

class HeightMap : public graphics::IHeightMap
{
public:

	HeightMap() = default;

	HeightMap(const std::vector<byte>& data, const uint32 width, const uint32 height)
	{
		image_ = generateFormattedHeightmap(data, width, height);
	}

	HeightMap(const IImage& image)
	{
		image_ = generateFormattedHeightmap(image);
	}

	HeightMap(const HeightMap& other)
	{
        if (other.image_)
        {
            image_ = std::make_unique<Image>(other.image_->data(), other.image_->width(), other.image_->height(), parseFormat(other.image_->format()));
        }
	}

	~HeightMap() override = default;

	HeightMap& operator=(const HeightMap& other)
	{
	    if (other.image_)
        {
            image_ = std::make_unique<Image>(other.image_->data(), other.image_->width(), other.image_->height(), parseFormat(other.image_->format()));
        }

		return *this;
	}

	byte height(uint32 x, uint32 z) const
	{
		if (x >= image_->width())  x %= image_->width();
		while (x < 0)    x += image_->width();
		if (z >= image_->height()) z %= image_->height();
		while (z < 0)    z += image_->height();

		return image_->data()[z*image_->width()*4 + x*4 + 3];
	}

	IImage* image()
	{
		return image_.get();
	}

	const graphics::IImage* image() const override
	{
		return image_.get();
	}

private:
	std::unique_ptr<IImage> image_;

    IImage::Format parseFormat(const int32 format) const
    {
        switch (format)
        {
            case IImage::Format::FORMAT_RGB:
                return IImage::Format::FORMAT_RGB;

            case IImage::Format::FORMAT_RGBA:
                return IImage::Format::FORMAT_RGBA;

            case IImage::Format::FORMAT_UNKNOWN:
            default:
                return IImage::Format::FORMAT_UNKNOWN;
        }
    }

	// Source:
	// https://stackoverflow.com/questions/2368728/can-normal-maps-be-generated-from-a-texture
	// and
	// http://www.catalinzima.com/2008/01/converting-displacement-maps-into-normal-maps/
	float32 height(const std::vector<byte>& data, const uint32 width, const uint32 height, uint32 x, uint32 z)
	{
		if (x >= width)  x %= width;
		while (x < 0)    x += width;
		if (z >= height) z %= height;
		while (z < 0)    z += height;

		return static_cast<float32>(data[z*width*4 + x*4 + 3]) / 255.0f;
	}

	byte textureCoordinateToRgb(const float32 value)
	{
	    return static_cast<byte>((value + 1.0f) * (255.0f / 2.0f));
	}

	glm::vec3 calculateNormal(const std::vector<byte>& data, const uint32 width, const uint32 height, const uint32 x, const uint32 z)
	{
		const float32 strength = 8.0f;

		// surrounding pixels
		float32 tl = this->height(data, width, height, x-1, z+1); // top left
		float32  l = this->height(data, width, height, x-1, z);   // left
		float32 bl = this->height(data, width, height, x-1, z-1); // bottom left
		float32  t = this->height(data, width, height, x, z+1);   // top
		float32  b = this->height(data, width, height, x, z-1);   // bottom
		float32 tr = this->height(data, width, height, x+1, z+1); // top right
		float32  r = this->height(data, width, height, x+1, z);   // right
		float32 br = this->height(data, width, height, x+1, z-1); // bottom right

		// sobel filter
		const float32 dX = (tr + 2.0f * r + br) - (tl + 2.0f * l + bl);
		const float32 dY = (bl + 2.0f * b + br) - (tl + 2.0f * t + tr);
		const float32 dZ = 1.0f / strength;

		glm::vec3 n(dX, dY, dZ);
		n = glm::normalize(n);

		return n;
	}

	void calculateNormals(std::vector<byte>& data, const uint32 width, const uint32 height)
	{
	    for (uint32 i = 0; i < width; ++i)
	    {
	        for (uint32 j = 0; j < height; ++j)
	        {
	           const glm::vec3 n = calculateNormal(data, width, height, i, j);

	            // convert to rgb
	            data[j*width*4 + i*4 + 0] = textureCoordinateToRgb(n.x);
	            data[j*width*4 + i*4 + 1] = textureCoordinateToRgb(n.y);
	            data[j*width*4 + i*4 + 2] = textureCoordinateToRgb(n.z);
	        }
	    }
	}

	std::unique_ptr<Image> generateFormattedHeightmap(const IImage& image)
    {
        return generateFormattedHeightmap(image.data(), image.width(), image.height(), image.format());
    }

	std::unique_ptr<Image> generateFormattedHeightmap(const std::vector<byte>& imageData, const uint32 width, const uint32 height, const int32 format = IImage::Format::FORMAT_RGB)
	{
		std::vector<byte> data;
		data.resize(width*height*4);

		std::unique_ptr<Image> newImage;

		if (format == IImage::Format::FORMAT_RGB)
		{
			int j=0;
			for (int i=0; i < data.size(); i+=4)
			{
				data[i] = imageData[j];
				data[i+1] = imageData[j+1];
				data[i+2] = imageData[j+2];
				data[i+3] = (imageData[j] + imageData[j+1] + imageData[j+2]) / 3;

				j+=3;
			}
		}
		else if (format == IImage::Format::FORMAT_RGBA)
		{
			for (int i=0; i < imageData.size(); i+=4)
			{
				data[i] = imageData[i];
				data[i+1] = imageData[i+1];
				data[i+2] = imageData[i+2];
				data[i+3] = (imageData[i] + imageData[i+1] + imageData[i+2]) / 3;
			}
		}

		calculateNormals(data, width, height);

		return std::make_unique<Image>(data, width, height, IImage::Format::FORMAT_RGBA);
	}
};

}

#endif /* HEIGHTMAP_H_ */
