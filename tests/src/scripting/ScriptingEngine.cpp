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
		fileSystem = ice_engine::fs::FileSystem();
		properties = ice_engine::utilities::Properties();
		logger = std::make_unique<ice_engine::logger::Logger>();
		
		scriptingEngine = std::make_unique<ice_engine::scripting::angel_script::ScriptingEngine>(&properties, &fileSystem, logger.get());
	}
	
	ice_engine::fs::FileSystem fileSystem;
	ice_engine::utilities::Properties properties;
	std::unique_ptr<ice_engine::logger::ILogger> logger;
	
	std::unique_ptr<ice_engine::scripting::angel_script::ScriptingEngine> scriptingEngine;
};

BOOST_FIXTURE_TEST_SUITE(ScriptingEngine, Fixture)

BOOST_AUTO_TEST_CASE(constructor)
{
}

BOOST_AUTO_TEST_CASE(executeScriptData)
{
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("void main() {}"), std::string("void main()")); );
}

BOOST_AUTO_TEST_CASE(executeScriptDataMultipleTimes)
{
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("void main() {}"), std::string("void main()")); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("void main() {}"), std::string("void main()")); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("void main() {}"), std::string("void main()")); );
}

BOOST_AUTO_TEST_CASE(ParameterFloat32)
{
	ice_engine::scripting::ParameterList params;
	params.add(1.0f);
	ice_engine::float32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string(std::string("float main(float input) { float i = input; return i; }")), std::string("float main(float)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1.0f);
}

BOOST_AUTO_TEST_CASE(ParameterFloat64)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::float64>(1.0);
	ice_engine::float64 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("double main(double input) { double i = input; return i; }"), std::string("double main(double)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1.0f);
}

BOOST_AUTO_TEST_CASE(ParameterInt8)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::int8>(1);
	ice_engine::int8 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int8 main(int8 input) { int8 i = input; return i; }"), std::string("int8 main(int8)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterUInt8)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::uint8>(1);
	ice_engine::uint8 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("uint8 main(uint8 input) { uint8 i = input; return i; }"), std::string("uint8 main(uint8)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterInt16)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::int16>(1);
	ice_engine::int16 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int16 main(int16 input) { int16 i = input; return i; }"), std::string("int16 main(int16)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterUInt16)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::uint16>(1);
	ice_engine::uint16 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("uint16 main(uint16 input) { uint16 i = input; return i; }"), std::string("uint16 main(uint16)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterInt32)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::int32>(1);
	ice_engine::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int32 main(int32 input) { int32 i = input; return i; }"), std::string("int32 main(int32)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterUInt32)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::uint32>(1);
	ice_engine::uint32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("uint32 main(uint32 input) { uint32 i = input; return i; }"), std::string("uint32 main(uint32)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterInt64)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::int64>(1);
	ice_engine::int64 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int64 main(int64 input) { int64 i = input; return i; }"), std::string("int64 main(int64)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterUInt64)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::uint64>(1);
	ice_engine::uint64 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("uint64 main(uint64 input) { uint64 i = input; return i; }"), std::string("uint64 main(uint64)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(ParameterByValue)
{
	ice_engine::scripting::ParameterList params;
	
	params.add(1.0f);
	
	auto returnObject = ice_engine::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("vec3 main(float f) { vec3 v; v.x = f; v.y = f; v.z = f; return v;}"), std::string("vec3 main(float)"), params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}

BOOST_AUTO_TEST_CASE(ParameterByValue2)
{
	ice_engine::scripting::ParameterList params;
	
	ice_engine::float32 f = 1.0f;
	params.add(f);
	
	auto returnObject = ice_engine::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("vec3 main(float f) { vec3 v; v.x = f; v.y = f; v.z = f; return v;}"), std::string("vec3 main(float)"), params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}

BOOST_AUTO_TEST_CASE(ParameterByReference)
{
	ice_engine::scripting::ParameterList params;
	
	ice_engine::float32 f = 1.0f;
	params.addRef(f);
	
	auto returnObject = ice_engine::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("vec3 main(float f) { vec3 v; v.x = f; v.y = f; v.z = f; return v;}"), std::string("vec3 main(float)"), params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}



// GLM TESTS
BOOST_AUTO_TEST_CASE(glmVec3Constructors)
{
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("void main() { vec3 v; }"), std::string("void main()")); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("void main() { vec3 v = vec3(); }"), std::string("void main()")); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("void main() { vec3 v = vec3(1.0, 2.0, 3.0); }"), std::string("void main()")); );
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("void main() { vec3 v = vec3( vec3(1.0, 2.0, 3.0) ); }"), std::string("void main()")); );
}

BOOST_AUTO_TEST_CASE(glmVec3ReturnFloat)
{
	ice_engine::float32 f = 0.0f;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("float main() { vec3 v; v.x = 1.0; v.y = 1.0; v.z = 1.0; return v.x;}"), std::string("float main()"), f); );
	
	BOOST_CHECK_EQUAL(f, 1.0f);
}

BOOST_AUTO_TEST_CASE(glmVec3ReturnGlmVec3)
{
	auto returnObject = ice_engine::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("vec3 main() { vec3 v; v.x = 1.0; v.y = 1.0; v.z = 1.0; return v;}"), std::string("vec3 main()"), returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}

BOOST_AUTO_TEST_CASE(glmVec3ParameterByValue)
{
	ice_engine::scripting::ParameterList params;
	
	glm::vec3 v = glm::vec3(1.0f, 1.0f, 1.0f);
	params.add(v);
	
	auto returnObject = ice_engine::scripting::ReturnObject<glm::vec3>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("vec3 main(vec3 vectorIn) { vec3 v = vectorIn; return v;}"), std::string("vec3 main(vec3)"), params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.x, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.y, 1.0f);
	BOOST_CHECK_EQUAL(returnObject.value.z, 1.0f);
}

BOOST_AUTO_TEST_CASE(glmVec3ParameterByReference)
{
	ice_engine::scripting::ParameterList params;
	
	glm::vec3 v = glm::vec3(1.0f, 1.0f, 1.0f);
	params.addRef(v);
	
	auto returnObject = ice_engine::scripting::ReturnObject<glm::vec3>();
	scriptingEngine->execute(std::string("vec3 main(vec3& in vectorIn) { vec3 v = vectorIn; return v;}"), std::string("vec3 main(vec3& in)"), params, returnObject.parser());
	
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
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("void main() { vectorInt v;}"), std::string("void main()")); );
}

BOOST_AUTO_TEST_CASE(vectorIntInitConstructor)
{
	ice_engine::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int32 main() { vectorInt v = vectorInt(10); return v.size(); }"), std::string("int32 main()"), returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 10);
}

BOOST_AUTO_TEST_CASE(vectorIntSize)
{
	ice_engine::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int32 main() { vectorInt v = vectorInt(); v.push_back(1); return v.size(); }"), std::string("int32 main()"), returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(vectorIntResize)
{
	ice_engine::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int32 main() { vectorInt v = vectorInt(); v.resize(1); return v.size(); }"), std::string("int32 main()"), returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(vectorIntAt)
{
	ice_engine::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int32 main() { vectorInt v; v.push_back(1); return v.at(0); }"), std::string("int32 main()"), returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(vectorInt32PushAndReturn)
{
	ice_engine::scripting::ParameterList params;
	params.add<ice_engine::int32>(1);
	ice_engine::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int32 main(int32 input) { vectorInt32 v = vectorInt32(); v.push_back(input); return v[0]; }"), std::string("int32 main(int32)"), params, returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 1);
}

BOOST_AUTO_TEST_CASE(vectorIntEraseAndReturn)
{
	ice_engine::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int32 main() { vectorInt v; v.push_back(1); v.erase(0); return v.size(); }"), std::string("int32 main()"), returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 0);
}

BOOST_AUTO_TEST_CASE(vectorIntInsertAndReturn)
{
	ice_engine::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int32 main() { vectorInt v; v.insert(0, 1); v.erase(0); return v.size(); }"), std::string("int32 main()"), returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 0);
}

BOOST_AUTO_TEST_CASE(vectorIntClear)
{
	ice_engine::int32 returnValue = 0;
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("int32 main() { vectorInt v = vectorInt(); v.push_back(1); v.clear(); return v.size(); }"), std::string("int32 main()"), returnValue); );
	BOOST_CHECK_EQUAL(returnValue, 0);
}

BOOST_AUTO_TEST_CASE(vectorIntReturn)
{
	auto returnObject = ice_engine::scripting::ReturnObject<std::vector<int>>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("vectorInt main() { vectorInt v; v.push_back(1); return v; }"), std::string("vectorInt main()"), returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.size(), 1);
	BOOST_CHECK_EQUAL(returnObject.value[0], 1);
}

BOOST_AUTO_TEST_CASE(vectorIntParameterByValue)
{
	ice_engine::scripting::ParameterList params;
	params.add(std::vector<int>());
	auto returnObject = ice_engine::scripting::ReturnObject<std::vector<int>>();
	BOOST_CHECK_NO_THROW( scriptingEngine->execute(std::string("vectorInt main(vectorInt v) { v.push_back(1); return v; }"), std::string("vectorInt main(vectorInt)"), params, returnObject.parser()); );
	
	BOOST_CHECK_EQUAL(returnObject.value.size(), 1);
	BOOST_CHECK_EQUAL(returnObject.value[0], 1);
}

/*
BOOST_AUTO_TEST_CASE(vectorIntParameterByReference)
{
	ice_engine::scripting::ParameterList params;
	
	std::vector<int> v;
	params.addRef(v);
	
	scriptingEngine->execute(std::string("void main(vectorInt@ v) { v.push_back(1); }"), std::string("void main(vectorInt@)"), params);
	
	BOOST_CHECK_EQUAL(v.size(), 1);
	BOOST_CHECK_EQUAL(v[0], 1);
}
*/

BOOST_AUTO_TEST_SUITE_END()
