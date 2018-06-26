#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include <string>
#include <memory>

#include "graphics/model/Model.hpp"

#include "logger/ILogger.hpp"
#include "fs/IFileSystem.hpp"

namespace ice_engine
{
namespace model
{

std::unique_ptr<graphics::model::Model> load(const std::string& filename, logger::ILogger* logger, fs::IFileSystem* fileSystem);

std::unique_ptr<graphics::model::Model> import(const std::string& filename, logger::ILogger* logger, fs::IFileSystem* fileSystem);

void save(const std::string& filename, const graphics::model::Model& model, logger::ILogger* logger, fs::IFileSystem* fileSystem);

}
}

#endif /* MODELLOADER_H_ */
