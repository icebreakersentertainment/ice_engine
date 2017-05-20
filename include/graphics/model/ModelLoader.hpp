#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include <string>
#include <memory>

#include "graphics/model/Model.hpp"

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

namespace hercules
{
namespace graphics
{
namespace model
{

std::unique_ptr<Model> load(const std::string& filename, logger::ILogger* logger, fs::IFileSystem* fileSystem);

std::unique_ptr<Model> import(const std::string& filename, logger::ILogger* logger, fs::IFileSystem* fileSystem);

void save(const std::string& filename, const Model& model, logger::ILogger* logger, fs::IFileSystem* fileSystem);

}
}
}

#endif /* MODELLOADER_H_ */
