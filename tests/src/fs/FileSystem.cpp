#define BOOST_TEST_MODULE FileSystem
#include <boost/test/unit_test.hpp>

#include <boost/filesystem.hpp>

#include "fs/FileSystem.hpp"

#include "exceptions/FileNotFoundException.hpp"

struct AutoDeletingFile
{
    AutoDeletingFile(const std::string& filename) : filename(filename)
    {
        std::ofstream outfile{filename};
    }

    ~AutoDeletingFile()
    {
        std::remove(filename.c_str());
    }

    AutoDeletingFile(const AutoDeletingFile&) = delete;
    AutoDeletingFile(AutoDeletingFile&&) = default;
    AutoDeletingFile& operator=(AutoDeletingFile other) = delete;
    AutoDeletingFile& operator=(AutoDeletingFile&& other) = default;

    std::string filename;
};

struct Fixture
{
	Fixture()
	{
		fileSystem = ice_engine::fs::FileSystem({boost::filesystem::current_path().string()});
		const auto mountedFile = boost::filesystem::current_path() / boost::filesystem::path("fixtures/mounted_file.txt");
		fileSystem.mountFile(mountedFile.string(), "mounted_file.txt");
	}
	
	ice_engine::fs::FileSystem fileSystem;
};

BOOST_FIXTURE_TEST_SUITE(FileSystem, Fixture)

BOOST_AUTO_TEST_CASE(constructor)
{
}

BOOST_AUTO_TEST_CASE(doesNotExist)
{
	const bool exists = fileSystem.exists("DOES_NOT_EXIST");

	BOOST_CHECK_EQUAL(exists, false);
}

BOOST_AUTO_TEST_CASE(doesFileExist)
{
	const bool exists = fileSystem.exists("fixtures/filesystem_test_file.txt");

	BOOST_CHECK_EQUAL(exists, true);
}

BOOST_AUTO_TEST_CASE(doesFileExistWithRelativeDirectory)
{
	const bool exists = fileSystem.exists("fixtures/../fixtures/filesystem_test_file.txt");

	BOOST_CHECK_EQUAL(exists, true);
}

BOOST_AUTO_TEST_CASE(fileOutsideOfMountedBaseDirectory)
{
    const auto tempDirPath = boost::filesystem::temp_directory_path();
    const auto tempFile = tempDirPath / boost::filesystem::unique_path();

    AutoDeletingFile adf{tempFile.string()};

    auto relativeTempFile = boost::filesystem::relative(tempFile);

	const bool exists = fileSystem.exists( relativeTempFile.string() );

	BOOST_CHECK_EQUAL(exists, false);
}

BOOST_AUTO_TEST_CASE(doesFolderExist)
{
	const bool exists = fileSystem.exists("fixtures");

	BOOST_CHECK_EQUAL(exists, true);
}

BOOST_AUTO_TEST_CASE(isNotDirectory)
{
	const bool isDirectory = fileSystem.isDirectory("fixtures/filesystem_test_file.txt");

	BOOST_CHECK_EQUAL(isDirectory, false);
}

BOOST_AUTO_TEST_CASE(isDirectory)
{
	const bool isDirectory = fileSystem.isDirectory( "fixtures");

	BOOST_CHECK_EQUAL(isDirectory, true);
}

BOOST_AUTO_TEST_CASE(deleteFile)
{
    const auto tempDirPath = boost::filesystem::temp_directory_path();

    fileSystem.mountBaseDirectory(tempDirPath.string());

    const auto tempFileName = boost::filesystem::unique_path();
    const auto tempFile = tempDirPath / tempFileName;

    AutoDeletingFile adf{tempFile.string()};

	fileSystem.deleteFile(tempFileName.string());
	
	const bool exists = fileSystem.exists(tempFile.string());

	BOOST_CHECK_EQUAL(exists, false);
}

BOOST_AUTO_TEST_CASE(read)
{
    auto file = fileSystem.open("fixtures/filesystem_test_file.txt", ice_engine::fs::FileFlags::READ);

    const auto content = file->read(256);

    BOOST_CHECK_EQUAL(content, "testing");
}

BOOST_AUTO_TEST_CASE(readAll)
{
    const auto content = fileSystem.readAll("fixtures/filesystem_test_file.txt");

    BOOST_CHECK_EQUAL(content, "testing");
}

BOOST_AUTO_TEST_CASE(readAllFileDoesNotExist)
{
    fileSystem = ice_engine::fs::FileSystem();

    BOOST_CHECK_THROW(fileSystem.readAll("fixtures/filesystem_test_file.txt"), ice_engine::FileNotFoundException);
}

BOOST_AUTO_TEST_CASE(readAllMountedFile)
{
    const auto content = fileSystem.readAll("mounted_file.txt");

    BOOST_CHECK_EQUAL(content, "testing");
}

BOOST_AUTO_TEST_SUITE_END()
