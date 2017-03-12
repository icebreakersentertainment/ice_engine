#define BOOST_TEST_MODULE FileSystem
#include <boost/test/unit_test.hpp>

#include "fs/FileSystem.hpp"

BOOST_AUTO_TEST_SUITE(FileSystem)

BOOST_AUTO_TEST_CASE(constructor)
{
	auto fileSystem = hercules::fs::FileSystem();
}

BOOST_AUTO_TEST_CASE(doesNotExist)
{
	auto fileSystem = hercules::fs::FileSystem();
	bool exists = fileSystem.exists( std::string("./DOES_NOT_EXIST") );

	BOOST_CHECK_EQUAL(exists, false);
}

BOOST_AUTO_TEST_CASE(doesExist)
{
	auto fileSystem = hercules::fs::FileSystem();
	bool exists = fileSystem.exists( std::string("./fixtures/filesystem_test_file.txt") );

	BOOST_CHECK_EQUAL(exists, true);
}

BOOST_AUTO_TEST_SUITE_END()
