#ifndef IRESOURCEIMPORTERFACTORY_HPP_
#define IRESOURCEIMPORTERFACTORY_HPP_

#include <memory>

#include "IResourceImporter.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace ice_engine
{

template<typename T>
class IResourceImporterFactory
{
public:
    virtual ~IResourceImporterFactory() = default;

    virtual std::unique_ptr<IResourceImporter<T>> create(utilities::Properties* properties, fs::IFileSystem* fileSystem, logger::ILogger* logger) const = 0;
};

}

#endif //IRESOURCEIMPORTERFACTORY_HPP_
