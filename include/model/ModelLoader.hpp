#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include <string>
#include <memory>

#include "model/Model.hpp"

#include "logger/ILogger.hpp"

namespace hercules
{
namespace model
{

std::unique_ptr<Model> load(const std::string& name, const std::string& filename, logger::ILogger* logger);

std::unique_ptr<Model> import(const std::string& name, const std::string& filename, logger::ILogger* logger);

void save(const std::string& name, const std::string& filename, std::unique_ptr<Model> model, logger::ILogger* logger);

}
}

#endif /* MODELLOADER_H_ */
