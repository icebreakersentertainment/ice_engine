#include <memory>

#define BOOST_TEST_MODULE AngelscriptCPreProcessor
#include <boost/test/unit_test.hpp>

#include "fs/FileSystem.hpp"
#include "logger/Logger.hpp"

#include "scripting/angel_script/AngelscriptCPreProcessor.hpp"

struct Fixture
{
    Fixture()
    {
        const auto includePath = boost::filesystem::current_path() / boost::filesystem::path("fixtures/c_pre_processor");

        fileSystem = ice_engine::fs::FileSystem({includePath.string()});
        logger = std::make_unique<ice_engine::logger::Logger>();
        cpp = std::unique_ptr<ice_engine::scripting::angel_script::AngelscriptCPreProcessor>(new ice_engine::scripting::angel_script::AngelscriptCPreProcessor(&fileSystem, logger.get()));
    }

    ice_engine::fs::FileSystem fileSystem;
    std::unique_ptr<ice_engine::logger::ILogger> logger;
    std::unique_ptr<ice_engine::scripting::angel_script::AngelscriptCPreProcessor> cpp;
};

BOOST_FIXTURE_TEST_SUITE(AngelscriptCPreProcessor, Fixture)

BOOST_AUTO_TEST_CASE(constructor)
{
}

BOOST_AUTO_TEST_CASE(processExpandIncludeAutoIncludeGuard1Include)
{
    const std::string source = R"(
#include "test_include.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test()
{

}

int main()
{
    return 0;
}
)";

    const std::string result = cpp->process(source, {}, true, true);

    BOOST_CHECK_EQUAL(result, expected);

    const auto processedSources = cpp->getProcessedFileSources();

    const std::string processedSourceExpected0 = R"(void test()
{

}
)";

    const std::string processedSourceExpected1 = R"(


int main()
{
    return 0;
}
)";

    BOOST_CHECK_EQUAL(processedSources.size(), 2);
    BOOST_CHECK_EQUAL(processedSources[0].second, processedSourceExpected0);
    BOOST_CHECK_EQUAL(processedSources[1].second, processedSourceExpected1);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeAutoIncludeGuard2Includes)
{
    const std::string source = R"(
#include "test_include.hpp"
#include "test_include_1.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test()
{

}
void test1()
{

}

int main()
{
    return 0;
}
)";

    const std::string result = cpp->process(source, {}, true, true);

    BOOST_CHECK_EQUAL(result, expected);

    const auto processedSources = cpp->getProcessedFileSources();

    const std::string processedSourceExpected = R"(



int main()
{
    return 0;
}
)";

    BOOST_CHECK_EQUAL(processedSources.size(), 3);
    BOOST_CHECK_EQUAL(processedSources[2].second, processedSourceExpected);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeAutoIncludeGuard3Includes)
{
    const std::string source = R"(
#include "test_include.hpp"
#include "test_include_1.hpp"

#include "test_include_2.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test()
{

}
void test1()
{

}

void test2()
{

}

int main()
{
    return 0;
}
)";

    const std::string result = cpp->process(source, {}, true, true);

    BOOST_CHECK_EQUAL(result, expected);

    const auto processedSources = cpp->getProcessedFileSources();

    const std::string processedSourceExpected = R"(





int main()
{
    return 0;
}
)";

    BOOST_CHECK_EQUAL(processedSources.size(), 4);
    BOOST_CHECK_EQUAL(processedSources[3].second, processedSourceExpected);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeAutoIncludeGuardLotsOfIncludes)
{
    const std::string source = R"(
#include "test_include.hpp"
#include "test_include_1.hpp"
#include "test_include_2.hpp"
#include "test_include_3.hpp"

#include "test_include_4.hpp"
#include "test_include_5.hpp"

#include "test_include_6.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test()
{

}
void test1()
{

}
void test2()
{

}
void test3()
{

}

void test4()
{

}
void test5()
{

}

void test6()
{

}

int main()
{
    return 0;
}
)";

    const std::string result = cpp->process(source, {}, true, true);

    BOOST_CHECK_EQUAL(result, expected);

    const auto processedSources = cpp->getProcessedFileSources();

    const std::string processedSourceExpected = R"(










int main()
{
    return 0;
}
)";

    BOOST_CHECK_EQUAL(processedSources.size(), 8);
    BOOST_CHECK_EQUAL(processedSources[7].second, processedSourceExpected);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeAutoIncludeGuard1DuplicatedInclude)
{
    const std::string source = R"(
#include "test_include.hpp"
#include "test_include.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test()
{

}


int main()
{
    return 0;
}
)";

    const std::string result = cpp->process(source, {}, true, true);

    BOOST_CHECK_EQUAL(result, expected);

    const auto processedSources = cpp->getProcessedFileSources();

    const std::string processedSourcesExpected0 = R"(void test()
{

}
)";

    const std::string processedSourcesExpected1 = R"(



int main()
{
    return 0;
}
)";

    const auto includePath = boost::filesystem::current_path() / "fixtures/c_pre_processor";

    auto processedSourcesCanonicalPath0 = boost::filesystem::weakly_canonical(includePath / "test_include.hpp");
    processedSourcesCanonicalPath0.make_preferred();
    const auto processedSourcesCanonicalPathExpected0 = processedSourcesCanonicalPath0.string();

    const std::string processedSourcesCanonicalPathExpected1 = "";

    BOOST_CHECK_EQUAL(processedSources.size(), 2);
    BOOST_CHECK_EQUAL(processedSources[0].first, processedSourcesCanonicalPathExpected0);
    BOOST_CHECK_EQUAL(processedSources[1].first, processedSourcesCanonicalPathExpected1);
    BOOST_CHECK_EQUAL(processedSources[0].second, processedSourcesExpected0);
    BOOST_CHECK_EQUAL(processedSources[1].second, processedSourcesExpected1);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeAutoIncludeGuard2DuplicatedIncludes)
{
    const std::string source = R"(
#include "test_include.hpp"
#include "test_include.hpp"
#include "test_include.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test()
{

}



int main()
{
    return 0;
}
)";

    const std::string result = cpp->process(source, {}, true, true);

    BOOST_CHECK_EQUAL(result, expected);

    const auto processedSources = cpp->getProcessedFileSources();

    const std::string processedSourcesExpected0 = R"(void test()
{

}
)";

    const std::string processedSourcesExpected1 = R"(




int main()
{
    return 0;
}
)";

    const auto includePath = boost::filesystem::current_path() / "fixtures/c_pre_processor";

    auto processedSourcesCanonicalPath0 = boost::filesystem::weakly_canonical(includePath / "test_include.hpp");
    processedSourcesCanonicalPath0.make_preferred();
    const auto processedSourcesCanonicalPathExpected0 = processedSourcesCanonicalPath0.string();

    const std::string processedSourcesCanonicalPathExpected1 = "";

    BOOST_CHECK_EQUAL(processedSources.size(), 2);
    BOOST_CHECK_EQUAL(processedSources[0].first, processedSourcesCanonicalPathExpected0);
    BOOST_CHECK_EQUAL(processedSources[1].first, processedSourcesCanonicalPathExpected1);
    BOOST_CHECK_EQUAL(processedSources[0].second, processedSourcesExpected0);
    BOOST_CHECK_EQUAL(processedSources[1].second, processedSourcesExpected1);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeAutoIncludeGuard3DuplicatedIncludes)
{
    const std::string source = R"(
#include "1/test_include_1.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test1()
{

}

void test2()
{

}




int main()
{
    return 0;
}
)";

    const std::string result = cpp->process(source, {}, true, true);

    BOOST_CHECK_EQUAL(result, expected);

    const auto processedSources = cpp->getProcessedFileSources();

    const std::string processedSourcesExpected0 = R"(void test2()
{

}
)";

    const std::string processedSourcesExpected1 = R"(void test1()
{

}





)";

    const std::string processedSourcesExpected2 = R"(


int main()
{
    return 0;
}
)";

    const auto includePath = boost::filesystem::current_path() / "fixtures/c_pre_processor";

    auto processedSourcesCanonicalPath0 = boost::filesystem::weakly_canonical(includePath / "1/2/test_include_2.hpp");
    processedSourcesCanonicalPath0.make_preferred();
    const auto processedSourcesCanonicalPathExpected0 = processedSourcesCanonicalPath0.string();
    auto processedSourcesCanonicalPath1 = boost::filesystem::weakly_canonical(includePath / "1/test_include_1.hpp");
    processedSourcesCanonicalPath1.make_preferred();
    const auto processedSourcesCanonicalPathExpected1 = processedSourcesCanonicalPath1.string();

    const std::string processedSourcesCanonicalPathExpected2 = "";

    BOOST_CHECK_EQUAL(processedSources.size(), 3);
    BOOST_CHECK_EQUAL(processedSources[0].first, processedSourcesCanonicalPathExpected0);
    BOOST_CHECK_EQUAL(processedSources[1].first, processedSourcesCanonicalPathExpected1);
    BOOST_CHECK_EQUAL(processedSources[2].first, processedSourcesCanonicalPathExpected2);
    BOOST_CHECK_EQUAL(processedSources[0].second, processedSourcesExpected0);
    BOOST_CHECK_EQUAL(processedSources[1].second, processedSourcesExpected1);
    BOOST_CHECK_EQUAL(processedSources[2].second, processedSourcesExpected2);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeAutoIncludeGuardManyDuplicatedIncludes)
{
    const std::string source = R"(
#include "test_include.hpp"
#include "1/test_include_1.hpp"
#include "1/test_include_1.hpp"
#include "1/test_include_1.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test()
{

}
void test1()
{

}

void test2()
{

}






int main()
{
    return 0;
}
)";

    const std::string result = cpp->process(source, {}, true, true);

    BOOST_CHECK_EQUAL(result, expected);

    const auto processedSources = cpp->getProcessedFileSources();

    const std::string processedSourcesExpected0 = R"(void test()
{

}
)";

    const std::string processedSourcesExpected1 = R"(void test2()
{

}
)";

    const std::string processedSourcesExpected2 = R"(void test1()
{

}





)";

    const std::string processedSourcesExpected3 = R"(





int main()
{
    return 0;
}
)";

    const auto includePath = boost::filesystem::current_path() / "fixtures/c_pre_processor";

    auto processedSourcesCanonicalPath0 = boost::filesystem::weakly_canonical(includePath / "test_include.hpp");
    processedSourcesCanonicalPath0.make_preferred();
    const auto processedSourcesCanonicalPathExpected0 = processedSourcesCanonicalPath0.string();

    auto processedSourcesCanonicalPath1 = boost::filesystem::weakly_canonical(includePath / "1/2/test_include_2.hpp");
    processedSourcesCanonicalPath1.make_preferred();
    const auto processedSourcesCanonicalPathExpected1 = processedSourcesCanonicalPath1.string();

    auto processedSourcesCanonicalPath2 = boost::filesystem::weakly_canonical(includePath / "1/test_include_1.hpp");
    processedSourcesCanonicalPath2.make_preferred();
    const auto processedSourcesCanonicalPathExpected2 = processedSourcesCanonicalPath2.string();

    const std::string processedSourcesCanonicalPathExpected3 = "";

    BOOST_CHECK_EQUAL(processedSources.size(), 4);
    BOOST_CHECK_EQUAL(processedSources[0].first, processedSourcesCanonicalPathExpected0);
    BOOST_CHECK_EQUAL(processedSources[1].first, processedSourcesCanonicalPathExpected1);
    BOOST_CHECK_EQUAL(processedSources[2].first, processedSourcesCanonicalPathExpected2);
    BOOST_CHECK_EQUAL(processedSources[3].first, processedSourcesCanonicalPathExpected3);
    BOOST_CHECK_EQUAL(processedSources[0].second, processedSourcesExpected0);
    BOOST_CHECK_EQUAL(processedSources[1].second, processedSourcesExpected1);
    BOOST_CHECK_EQUAL(processedSources[2].second, processedSourcesExpected2);
    BOOST_CHECK_EQUAL(processedSources[3].second, processedSourcesExpected3);
}

BOOST_AUTO_TEST_SUITE_END()
