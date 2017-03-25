#define BOOST_TEST_MODULE ScriptingEngine
#include <boost/test/unit_test.hpp>

#include "fs/FileSystem.hpp"
#include "utilities/Properties.hpp"
#include "logger/Logger.hpp"

#include "scripting/angel_script/ScriptingEngine.hpp"

struct Fixture
{
	Fixture()
	{
		fileSystem = hercules::fs::FileSystem();
		properties = hercules::utilities::Properties();
		logger = std::make_unique<hercules::logger::Logger>();
		
		scriptingEngine = std::make_unique<hercules::scripting::angel_script::ScriptingEngine>(&properties, &fileSystem, logger.get());
	}
	
	hercules::fs::FileSystem fileSystem;
	hercules::utilities::Properties properties;
	std::unique_ptr<hercules::logger::ILogger> logger;
	
	std::unique_ptr<hercules::scripting::angel_script::ScriptingEngine> scriptingEngine;
};

BOOST_FIXTURE_TEST_SUITE(ScriptingEngine, Fixture)

BOOST_AUTO_TEST_CASE(constructor)
{
}

BOOST_AUTO_TEST_CASE(executeScriptData)
{
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() { println(\"testing1234\n\"); }"); );
}

BOOST_AUTO_TEST_CASE(executeScriptDataMultipleTimes)
{
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() {}"); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() {}"); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() {}"); );
}

BOOST_AUTO_TEST_SUITE_END()
