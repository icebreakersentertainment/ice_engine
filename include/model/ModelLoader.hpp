#ifndef MODELLOADER_H_
#define MODELLOADER_H_

#include <string>
#include <memory>

#include "model/Model.hpp"

namespace model
{

std::unique_ptr<Model> load(const std::string& name, const std::string& filename);

std::unique_ptr<Model> import(const std::string& name, const std::string& filename);

void save(const std::string& name, const std::string& filename, std::unique_ptr<Model> model);

}

#endif /* MODELLOADER_H_ */
