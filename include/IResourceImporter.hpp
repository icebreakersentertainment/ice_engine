#ifndef IRESOURCEIMPORTER_HPP_
#define IRESOURCEIMPORTER_HPP_

#include <string>
#include <memory>

namespace ice_engine
{

template<typename T>
class IResourceImporter
{
public:
    virtual ~IResourceImporter() = default;

    virtual std::unique_ptr<T> import(const std::string& name, const std::string& filename) const = 0;

    virtual bool supports(const std::string& filename) const = 0;
};

}

#endif //IRESOURCEIMPORTER_HPP_
