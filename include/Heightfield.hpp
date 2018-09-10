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
	
	Heightfield(const Image& image)
	{
		generateHeightfield(image);
	}
	
	virtual ~Heightfield() = default;

	virtual const std::vector<char>& data() const override
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
			return 15.0f;
		}

private:
	std::vector<char> data_;
	uint32 width_ = 0.0f;
	uint32 length_ = 0.0f;
	uint32 height_ = 0.0f;

	void generateHeightfield(const Image& image)
	{
		const int32 dataSize = (image.format() == Image::Format::FORMAT_RGBA ? 4 : 3);

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
