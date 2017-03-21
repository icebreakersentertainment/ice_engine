#include "scripting/ScriptingFactory.hpp"

#include "scripting/angel_script/ScriptingEngine.hpp"

namespace hercules
{
namespace scripting
{

ScriptingFactory::ScriptingFactory()
{
}

ScriptingFactory::ScriptingFactory(const ScriptingFactory& other)
{
}

ScriptingFactory::~ScriptingFactory()
{
}

std::unique_ptr<IScriptingEngine> ScriptingFactory::createScriptingEngine(
	utilities::Properties* properties,
	fs::IFileSystem* fileSystem,
	logger::ILogger* logger
)
{
	auto ptr = std::unique_ptr< IScriptingEngine >( new angel_script::ScriptingEngine(properties, fileSystem, logger) );
	
	return std::move( ptr );
}

}
}
