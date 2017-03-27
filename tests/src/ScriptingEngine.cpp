#define BOOST_TEST_MODULE ScriptingEngine
#include <boost/test/unit_test.hpp>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "fs/FileSystem.hpp"
#include "utilities/Properties.hpp"
#include "logger/Logger.hpp"

#include "scripting/angel_script/ScriptingEngine.hpp"
#include "scripting/ReturnObject.hpp"
#include "scripting/ParameterList.hpp"

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

BOOST_AUTO_TEST_CASE(glmVec3ReturnFloat)
{
	hercules::float32 f = 0.0f;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("float main() { vec3 v; v.x = 1.0; v.y = 1.0; v.z = 1.0; return v.x;}", "float main()", f); );
	
	BOOST_CHECK_EQUAL(f, 1.0f);
}

BOOST_AUTO_TEST_CASE(glmVec3ReturnGlmVec3)
{
	auto returnObject = hercules::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("vec3 main() { vec3 v; v.x = 1.0; v.y = 1.0; v.z = 1.0; return v;}", "vec3 main()", returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}

BOOST_AUTO_TEST_CASE(ParameterInt)
{
	hercules::scripting::ParameterList params;
	params.add(1);
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int main(int input) { int i = input; return i; }", "int main(int)", params, returnValue); );
}

BOOST_AUTO_TEST_CASE(ParameterByValue)
{
	hercules::scripting::ParameterList params;
	
	params.add(1.0f);
	
	auto returnObject = hercules::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("vec3 main(float f) { vec3 v; v.x = f; v.y = f; v.z = f; return v;}", "vec3 main(float)", params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}

BOOST_AUTO_TEST_CASE(ParameterByValue2)
{
	hercules::scripting::ParameterList params;
	
	hercules::float32 f = 1.0f;
	params.add(f);
	
	auto returnObject = hercules::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("vec3 main(float f) { vec3 v; v.x = f; v.y = f; v.z = f; return v;}", "vec3 main(float)", params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}

BOOST_AUTO_TEST_CASE(ParameterByReference)
{
	hercules::scripting::ParameterList params;
	
	hercules::float32 f = 1.0f;
	params.addRef(f);
	
	auto returnObject = hercules::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("vec3 main(float f) { vec3 v; v.x = f; v.y = f; v.z = f; return v;}", "vec3 main(float)", params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}

BOOST_AUTO_TEST_CASE(glmVec3ParameterByValue)
{
	hercules::scripting::ParameterList params;
	
	glm::vec3 v = glm::vec3(1.0f, 1.0f, 1.0f);
	params.add(v);
	
	auto returnObject = hercules::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("vec3 main(vec3 vectorIn) { vec3 v = vectorIn; return v;}", "vec3 main(vec3)", params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}

BOOST_AUTO_TEST_CASE(glmVec3ParameterByReference)
{
	hercules::scripting::ParameterList params;
	
	glm::vec3 v = glm::vec3(1.0f, 1.0f, 1.0f);
	params.addRef(v);
	
	auto returnObject = hercules::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("vec3 main(vec3 vectorIn) { vec3 v = vectorIn; return v;}", "vec3 main(vec3)", params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
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
