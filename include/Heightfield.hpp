#ifndef HEIGHTFIELD_H_
#define HEIGHTFIELD_H_

#include <vector>

#include "physics/IHeightfield.hpp"

#include "Image.hpp"

namespace ice_engine
{

class Heightfield : public physics::IHeightfield
{
public:

	Heightfield() = default;

	Heightfield(const IImage& image)
	{
		generateHeightfield(image);
	}

	virtual ~Heightfield() override = default;

	virtual const std::vector<byte>& data() const override
	{
        return data_;
	}

	virtual uint32 width() const override
    {
        return width_;
    }

    virtual uint32 length() const override
    {
        return length_;
    }

    virtual uint32 height() const override
    {
//        return 15.0f;
        return 15;
    }

private:
	std::vector<byte> data_;
	uint32 width_ = 0;
	uint32 length_ = 0;
	uint32 height_ = 0;

	void generateHeightfield(const IImage& image)
	{
		const int32 dataSize = (image.format() == IImage::Format::FORMAT_RGBA ? 4 : 3);

		width_ = image.width();
		length_ = image.height();

		data_.resize(image.width() * image.height());

		int j=0;
		for (int i=0; i < data_.size(); ++i)
		{
			data_[i] = image.data()[j+3];

			j+=dataSize;
		}
	}
};

}

#endif /* HEIGHTFIELD_H_ */
