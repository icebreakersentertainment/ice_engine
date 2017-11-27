#include <celero/Celero.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "fs/FileSystem.hpp"
#include "utilities/Properties.hpp"
#include "logger/Logger.hpp"

#include "scripting/angel_script/ScriptingEngine.hpp"

CELERO_MAIN

class Fixture : public celero::TestFixture
{
public:
	virtual void setUp(int64_t experimentValue) override
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

class FixtureCallback : public Fixture
{
public:
	virtual void setUp(int64_t experimentValue) override
	{
		Fixture::setUp(experimentValue);
		
		scriptingEngine->registerInterface("ITest");
		scriptingEngine->registerInterfaceMethod("ITest", "void tick(const float)");
		
		scriptingEngine->registerGlobalFunction(
			"void setTestInstance(ITest@)",
			asMETHODPR(FixtureCallback, setTestInstance, (hercules::scripting::ScriptObjectHandle), void),
			asCALL_THISCALL_ASGLOBAL,
			this
		);
		
		auto source = R"END(
class Test : ITest
{
	private int tickCount = 0;

	void tick(const float value)
	{
		tickCount++;
	}
}

void main()
{
	ITest@ test = Test();
	setTestInstance(test);
}

)END";
		scriptHandle = scriptingEngine->loadScript(source);
		scriptingEngine->execute(scriptHandle, std::string("void main()"));
		
		scriptObjectFunctionHandle = scriptingEngine->getScriptObjectFunction(scriptObjectHandle, std::string("void tick(const float)"));
	}
	
	void setTestInstance(hercules::scripting::ScriptObjectHandle scriptObjectHandle)
	{
		this->scriptObjectHandle = scriptObjectHandle;
	}
	
	hercules::scripting::ScriptObjectHandle scriptObjectHandle;
	hercules::scripting::ScriptObjectFunctionHandle scriptObjectFunctionHandle;
	hercules::scripting::ScriptHandle scriptHandle;
};

BASELINE_F(ScriptingEngine, ExecuteScriptData, Fixture, 0, 10000)
{
	scriptingEngine->execute(std::string("void main() {}"), std::string("void main()"));
}

BENCHMARK_F(ScriptingEngine, ExecuteScriptData, Fixture, 0, 10000)
{
	scriptingEngine->execute(std::string("void main() {}"), std::string("void main()"));
}

BENCHMARK_F(ScriptingEngine, ScriptObjectTick, FixtureCallback, 0, 10000)
{
	hercules::scripting::ParameterList params;
	params.add(0.001f);
	
	scriptingEngine->execute(scriptObjectHandle, scriptObjectFunctionHandle, params);
}
