#ifndef ITEXTURE_H_
#define ITEXTURE_H_

#include <string>

#include "graphics/IImage.hpp"

namespace ice_engine
{
namespace graphics
{

class ITexture
{
public:
	virtual ~ITexture() = default;
	
	virtual const std::string& name() const = 0;
	virtual const IImage* image() const = 0;
};

}
}

#endif /* ITEXTURE_H_ */
