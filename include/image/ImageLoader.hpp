#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

#include <FreeImage.h>

#include "image/Image.hpp"

namespace ice_engine
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

	auto image = std::make_unique<Image>();
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
