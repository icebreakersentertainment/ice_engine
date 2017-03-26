#define BOOST_TEST_MODULE ScriptingEngine
#include <boost/test/unit_test.hpp>

#include "fs/FileSystem.hpp"
#include "utilities/Properties.hpp"
#include "logger/Logger.hpp"

#include "scripting/angel_script/ScriptingEngine.hpp"
#include "scripting/Variant.hpp"

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
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() {}"); );
}

BOOST_AUTO_TEST_CASE(executeScriptDataMultipleTimes)
{
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() {}"); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() {}"); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() {}"); );
}

BOOST_AUTO_TEST_CASE(glmVec3Constructors)
{
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() { vec3 v; }"); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() { vec3 v = vec3(); }"); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() { vec3 v = vec3(1.0, 2.0, 3.0); }"); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() { vec3 v = vec3( vec3(1.0, 2.0, 3.0) ); }"); );
}

BOOST_AUTO_TEST_CASE(glmVec3Assignment)
{
	auto variant = hercules::scripting::Variant<float>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("float main() { vec3 v; v.x = 1.0; v.y = 1.0; v.z = 1.0; return v.x;}", "float main()", variant.gen()); );
	
	BOOST_CHECK_EQUAL(variant.value, 1.0f);
}

BOOST_AUTO_TEST_CASE(glmVec3Functions)
{/*
	const char* R"SCRIPT(
void main()
{
	
	
}
	)SCRIPT"
	*/
}

BOOST_AUTO_TEST_SUITE_END()
