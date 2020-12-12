#ifndef ICEENGINE_IIMAGE_H_
#define ICEENGINE_IIMAGE_H_

#include <vector>

#include "graphics/IImage.hpp"
#include "physics/IImage.hpp"

namespace ice_engine
{

class IImage : public graphics::IImage, public physics::IImage
{
public:
    enum Format
    {
        FORMAT_UNKNOWN = -1,
        FORMAT_RGB,
        FORMAT_RGBA
    };

    ~IImage() override = default;

    const std::vector<byte>& data() const override = 0;

    uint32 width() const override = 0;
    uint32 height() const override = 0;

    int32 format() const override = 0;
};

}

#endif /* ICEENGINE_IIMAGE_H_ */
