#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include <string>
#include <memory>

#include "graphics/model/Model.hpp"

#include "logger/ILogger.hpp"

namespace hercules
{
namespace graphics
{
namespace model
{

Model load(const std::string& name, const std::string& filename, logger::ILogger* logger);

Model import(const std::string& name, const std::string& filename, logger::ILogger* logger);

void save(const std::string& name, const std::string& filename, const Model& model, logger::ILogger* logger);

}
}
}

#endif /* MODELLOADER_H_ */
