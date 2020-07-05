#ifndef ICEENGINE_IIMAGE_H_
#define ICEENGINE_IIMAGE_H_

#include <vector>

#include "graphics/IImage.hpp"
#include "physics/IImage.hpp"

namespace ice_engine
{

class IImage : public virtual graphics::IImage, public virtual physics::IImage
{
public:
    enum Format
    {
        FORMAT_UNKNOWN = -1,
        FORMAT_RGB,
        FORMAT_RGBA
    };

    virtual ~IImage() = default;

    virtual const std::vector<byte>& data() const = 0;

    virtual uint32 width() const = 0;
    virtual uint32 height() const = 0;

    virtual int32 format() const = 0;
};

}

#endif /* ICEENGINE_IIMAGE_H_ */
