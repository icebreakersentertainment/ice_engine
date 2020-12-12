#include <memory>

#define BOOST_TEST_MODULE CPreProcessor
#include <boost/test/unit_test.hpp>

#include "fs/FileSystem.hpp"
#include "logger/Logger.hpp"

#include "CPreProcessor.hpp"

struct Fixture
{
    Fixture()
    {
        const auto includePath = boost::filesystem::current_path() / boost::filesystem::path("fixtures/c_pre_processor");

        fileSystem = ice_engine::fs::FileSystem({includePath.string()});
        logger = std::make_unique<ice_engine::logger::Logger>();
        cpp = std::unique_ptr<ice_engine::CPreProcessor>(new ice_engine::CPreProcessor(&fileSystem, logger.get()));
    }

    ice_engine::fs::FileSystem fileSystem;
    std::unique_ptr<ice_engine::logger::ILogger> logger;
    std::unique_ptr<ice_engine::CPreProcessor> cpp;
};

BOOST_FIXTURE_TEST_SUITE(CPreProcessor, Fixture)

BOOST_AUTO_TEST_CASE(constructor)
{
}

BOOST_AUTO_TEST_CASE(processEmpty)
{
    const std::string result = cpp->process("");

    BOOST_CHECK_EQUAL(result, "");
}

BOOST_AUTO_TEST_CASE(processNoChanges)
{
    const std::string source = R"(
int main()
{
    return 0;
}
)";

    const std::string result = cpp->process(source);

    BOOST_CHECK_EQUAL(result, source);
}

BOOST_AUTO_TEST_CASE(processExpandDefine)
{
    const std::string source = R"(
#define TEST 1234

int main()
{
    return TEST;
}
)";

    const std::string expected = R"(

int main()
{
    return 1234;
}
)";

    const std::string result = cpp->process(source);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(processExpandIf)
{
    const std::string source = R"(
#if 1
void test1()
{

}
#else
void test2()
{

}
#endif

int main()
{
    return 1234;
}
)";

    const std::string expected = R"(
void test1()
{

}

int main()
{
    return 1234;
}
)";

    const std::string result = cpp->process(source);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(processExpandInclude)
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

    const std::string result = cpp->process(source);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeOverride)
{
    const std::string source = R"(
#include "test_include.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test1()
{

}

int main()
{
    return 0;
}
)";

    const std::string includeOverrideContents = R"(void test1()
{

}
)";

    const std::unordered_map<std::string, std::string> includeOverrides = {
            {"test_include.hpp", includeOverrideContents}
    };

    cpp = std::unique_ptr<ice_engine::CPreProcessor>(new ice_engine::CPreProcessor(&fileSystem, logger.get(), includeOverrides));
    const std::string result = cpp->process(source);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeNoAutoIncludeGuard)
{
    const std::string source = R"(
#include "test_include.hpp"
#include "1/2/test_include_2.hpp"
#include "1/2/test_include_2.hpp"

int main()
{
    return 0;
}
)";

    const std::string expected = R"(
void test()
{

}
void test2()
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

    cpp = std::unique_ptr<ice_engine::CPreProcessor>(new ice_engine::CPreProcessor(&fileSystem, logger.get(), {}));
    const std::string result = cpp->process(source, {}, false);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(processExpandIncludeAutoIncludeGuard)
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

    cpp = std::unique_ptr<ice_engine::CPreProcessor>(new ice_engine::CPreProcessor(&fileSystem, logger.get(), {}));
    const std::string result = cpp->process(source, {}, true);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(processExpandPreserveLineNumbers)
{
    const std::string source = R"(
#include "test_include.hpp"

#if 1
void test1()
{

}
#else
void test2()
{

}
#endif

#if 1
void test3()
{

}
#endif

int main()
{
    return 1234;
}
)";

    const std::string expected = R"(
void test()
{

}


void test1()
{

}








void test3()
{

}


int main()
{
    return 1234;
}
)";

    const std::string result = cpp->process(source, {}, false, true);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_CASE(processExpandAutoIncludeGuardAndPreserveLineNumbers)
{
    const std::string source = R"(
#include "test_include.hpp"
#include "test_include.hpp"
#include "test_include.hpp"

#if 1
void test1()
{

}
#if 1
void test2()
{

}
#endif
#else
void test3()
{

}
#endif

int main()
{
    return 1234;
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
    return 1234;
}
)";

    const std::string result = cpp->process(source, {}, true, true);

    BOOST_CHECK_EQUAL(result, expected);
}

BOOST_AUTO_TEST_SUITE_END()
