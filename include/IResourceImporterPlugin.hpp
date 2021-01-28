#ifndef IIMAGERESOURCEIMPORTERPLUGIN_HPP_
#define IIMAGERESOURCEIMPORTERPLUGIN_HPP_

#include <memory>

#include "IResourceImporterFactory.hpp"

#include "IPlugin.hpp"

namespace ice_engine
{

template<typename T>
class IResourceImporterPlugin : public IPlugin
{
public:
    virtual ~IResourceImporterPlugin() = default;

    virtual std::unique_ptr<IResourceImporterFactory<T>> createFactory() const = 0;

};

}

#endif //IIMAGERESOURCEIMPORTERPLUGIN_HPP_