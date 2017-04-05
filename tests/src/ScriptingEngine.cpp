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

BOOST_AUTO_TEST_CASE(ParameterFloat32)
{
	hercules::scripting::ParameterList params;
	params.add(1.0f);
	hercules::float32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("float main(float input) { float i = input; return i; }", "float main(float)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1.0f);
}

BOOST_AUTO_TEST_CASE(ParameterFloat64)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::float64>(1.0);
	hercules::float64 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("double main(double input) { double i = input; return i; }", "double main(double)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1.0f);
}

BOOST_AUTO_TEST_CASE(ParameterInt8)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::int8>(1);
	hercules::int8 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int8 main(int8 input) { int8 i = input; return i; }", "int8 main(int8)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterUInt8)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::uint8>(1);
	hercules::uint8 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("uint8 main(uint8 input) { uint8 i = input; return i; }", "uint8 main(uint8)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterInt16)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::int16>(1);
	hercules::int16 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int16 main(int16 input) { int16 i = input; return i; }", "int16 main(int16)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterUInt16)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::uint16>(1);
	hercules::uint16 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("uint16 main(uint16 input) { uint16 i = input; return i; }", "uint16 main(uint16)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterInt32)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::int32>(1);
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int32 main(int32 input) { int32 i = input; return i; }", "int32 main(int32)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterUInt32)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::uint32>(1);
	hercules::uint32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("uint32 main(uint32 input) { uint32 i = input; return i; }", "uint32 main(uint32)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterInt64)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::int64>(1);
	hercules::int64 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int64 main(int64 input) { int64 i = input; return i; }", "int64 main(int64)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterUInt64)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::uint64>(1);
	hercules::uint64 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("uint64 main(uint64 input) { uint64 i = input; return i; }", "uint64 main(uint64)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
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



// GLM TESTS
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
	scriptingEngine->execute("vec3 main(vec3& in vectorIn) { vec3 v = vectorIn; return v;}", "vec3 main(vec3& in)", params, returnObject.parser());
	
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



/* VECTOR TESTS */
BOOST_AUTO_TEST_CASE(vectorInt)
{
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("void main() { vectorInt v;}"); );
}

BOOST_AUTO_TEST_CASE(vectorIntInitConstructor)
{
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int32 main() { vectorInt v = vectorInt(10); return v.size(); }", "int32 main()", returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 10);
}

BOOST_AUTO_TEST_CASE(vectorIntSize)
{
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int32 main() { vectorInt v = vectorInt(); v.push_back(1); return v.size(); }", "int32 main()", returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(vectorIntResize)
{
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int32 main() { vectorInt v = vectorInt(); v.resize(1); return v.size(); }", "int32 main()", returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(vectorIntAt)
{
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int32 main() { vectorInt v; v.push_back(1); return v.at(0); }", "int32 main()", returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(vectorInt32PushAndReturn)
{
	hercules::scripting::ParameterList params;
	params.add<hercules::int32>(1);
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int32 main(int32 input) { vectorInt32 v = vectorInt32(); v.push_back(input); return v[0]; }", "int32 main(int32)", params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(vectorIntEraseAndReturn)
{
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int32 main() { vectorInt v; v.push_back(1); v.erase(0); return v.size(); }", "int32 main()", returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 0);
}

BOOST_AUTO_TEST_CASE(vectorIntInsertAndReturn)
{
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int32 main() { vectorInt v; v.insert(0, 1); v.erase(0); return v.size(); }", "int32 main()", returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 0);
}

BOOST_AUTO_TEST_CASE(vectorIntClear)
{
	hercules::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("int32 main() { vectorInt v = vectorInt(); v.push_back(1); v.clear(); return v.size(); }", "int32 main()", returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 0);
}

BOOST_AUTO_TEST_CASE(vectorIntReturn)
{
	auto returnObject = hercules::scripting::ReturnObject<std::vector<int>>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("vectorInt main() { vectorInt v; v.push_back(1); return v; }", "vectorInt main()", returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.size(), 1);
	BOOST_CHECK_EQUAL(returnObject.value[0], 1);
}

BOOST_AUTO_TEST_CASE(vectorIntParameterByValue)
{
	hercules::scripting::ParameterList params;
	params.add(std::vector<int>());
	auto returnObject = hercules::scripting::ReturnObject<std::vector<int>>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute("vectorInt main(vectorInt v) { v.push_back(1); return v; }", "vectorInt main(vectorInt)", params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.size(), 1);
	BOOST_CHECK_EQUAL(returnObject.value[0], 1);
}

/*
BOOST_AUTO_TEST_CASE(vectorIntParameterByReference)
{
	hercules::scripting::ParameterList params;
	
	std::vector<int> v;
	params.addRef(v);
	
	scriptingEngine->execute("void main(vectorInt@ v) { v.push_back(1); }", "void main(vectorInt@)", params);
	
	BOOST_CHECK_EQUAL(v.size(), 1);
	BOOST_CHECK_EQUAL(v[0], 1);
}
*/

BOOST_AUTO_TEST_SUITE_END()
