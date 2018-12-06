#ifndef IMAGEUTILITIES_H_
#define IMAGEUTILITIES_H_

#include <vector>

#include "fs/IFile.hpp"

#include "Image.hpp"

namespace ice_engine
{
namespace utilities
{

// Source: http://www.flashbang.se/archives/155
inline void saveImageToFile(fs::IFile& file, const Image& image)
{
	std::vector<byte> data;
	if (image.format() == Image::Format::FORMAT_RGBA)
	{
		// Convert from RGBA to RGB
		data.resize(image.data().size()/4 * 3);

		int j=0;
		for (int i=0; i < data.size(); i+=3)
		{
			data[i] = image.data()[j];
			data[i+1] = image.data()[j+1];
			data[i+2] = image.data()[j+2];

			j+=4;
		}
	}
	else
	{
		data = image.data();
	}

	/*
	BYTE pixels [3*257*257];
	FIBITMAP* Image = FreeImage_ConvertFromRawBits(reinterpret_cast<BYTE*>(&data[0]), image.width(), image.height(), 3*image.width(), 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_BMP, Image, "../assets/test.bmp", 0);
	*/

	int xa= image.width() % 256;
	int xb= (image.width()-xa)/256;
	int ya= image.height() % 256;
	int yb= (image.height()-ya)/256;//assemble the header
	unsigned char header[18]={0,0,2,0,0,0,0,0,0,0,0,0,(unsigned char)xa,(unsigned char)xb,(unsigned char)ya,(unsigned char)yb,24,0};

	// write header and data to file
	auto& ostream = file.getOutputStream();
	ostream.write (reinterpret_cast<char *>(header), sizeof (char)*18);
	ostream.write (reinterpret_cast<char *>(&data[0]), sizeof (char)*data.size());
}

}
}

#endif /* IMAGEUTILITIES_H_ */
