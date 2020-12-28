from conans import ConanFile, CMake, tools


class CtplConan(ConanFile):
    name = "ctpl"
    version = "0.0.2"
    license = "Apache-2.0"
    author = "<Put your name here> <And your email here>"
    url = "https://github.com/icebreakersentertainment/CTPL"
    description = "<Description of Ctpl here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
#     requires = "boost/[>=1.69.0]"
    requires = "boost/1.69.0"
    exports_sources = "include/*"
    no_copy_source = True

    def source(self):
        self.run("git clone https://github.com/icebreakersentertainment/CTPL.git")
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly
#         tools.replace_in_file("hello/CMakeLists.txt", "PROJECT(HelloWorld)",
#                               '''PROJECT(HelloWorld)
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()''')

    def package(self):
        self.copy("*.h", dst="include", src="CTPL")
        pass

    def package_id(self):
        self.info.header_only()
