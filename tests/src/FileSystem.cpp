#define BOOST_TEST_MODULE FileSystem
#include <boost/test/unit_test.hpp>

#include "fs/FileSystem.hpp"

struct Fixture
{
	Fixture()
	{
		fileSystem = ice_engine::fs::FileSystem();
	}
	
	ice_engine::fs::FileSystem fileSystem;
};

BOOST_FIXTURE_TEST_SUITE(FileSystem, Fixture)

BOOST_AUTO_TEST_CASE(constructor)
{
}

BOOST_AUTO_TEST_CASE(doesNotExist)
{
	bool exists = fileSystem.exists( std::string("./DOES_NOT_EXIST") );

	BOOST_CHECK_EQUAL(exists, false);
}

BOOST_AUTO_TEST_CASE(doesFileExist)
{
	bool exists = fileSystem.exists( std::string("./fixtures/filesystem_test_file.txt") );

	BOOST_CHECK_EQUAL(exists, true);
}

BOOST_AUTO_TEST_CASE(doesFolderExist)
{
	bool exists = fileSystem.exists( std::string("./fixtures") );

	BOOST_CHECK_EQUAL(exists, true);
}

BOOST_AUTO_TEST_CASE(isNotDirectory)
{
	bool isDirectory = fileSystem.isDirectory( std::string("./fixtures/filesystem_test_file.txt") );

	BOOST_CHECK_EQUAL(isDirectory, false);
}

BOOST_AUTO_TEST_CASE(isDirectory)
{
	bool isDirectory = fileSystem.isDirectory( std::string("./fixtures") );

	BOOST_CHECK_EQUAL(isDirectory, true);
}

BOOST_AUTO_TEST_CASE(deleteFile)
{
	//fileSystem.createFile( std::string("./fixtures/delete_me.txt") );
	//fileSystem.deleteFile( std::string("./fixtures/delete_me.txt") );
	
	//bool exists = fileSystem.exists( std::string("./fixtures/delete_me.txt") );

	//BOOST_CHECK_EQUAL(exists, false);
}

BOOST_AUTO_TEST_SUITE_END()
