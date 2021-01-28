#ifndef IRESOURCEIMPORTER_IMAGE_HPP_
#define IRESOURCEIMPORTER_IMAGE_HPP_

#include <string>
#include <memory>

#include "Image.hpp"

namespace ice_engine
{

template<>
class IResourceImporter<Image>
{
public:
    virtual ~IResourceImporter() = default;

    virtual std::unique_ptr<Image> import(const std::string& name, const std::string& filename) const = 0;

    virtual bool supported(const std::string& filename) const = 0;
};

}

#endif //IRESOURCEIMPORTER_IMAGE_HPP_
