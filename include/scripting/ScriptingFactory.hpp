#ifndef SCRIPTINGFACTORY_H_
#define SCRIPTINGFACTORY_H_

#include <memory>

#include "IScriptingEngine.hpp"

#include "utilities/Properties.hpp"
#include "fs/IFileSystem.hpp"
#include "logger/ILogger.hpp"

namespace hercules
{
namespace scripting
{

class ScriptingFactory
{
public:

	static std::unique_ptr<IScriptingEngine> createScriptingEngine(
		utilities::Properties* properties,
		fs::IFileSystem* fileSystem,
		logger::ILogger* logger
	);

private:
	ScriptingFactory();
	ScriptingFactory(const ScriptingFactory& other);
	virtual ~ScriptingFactory();
};

}

}

#endif /* SCRIPTINGFACTORY_H_ */
