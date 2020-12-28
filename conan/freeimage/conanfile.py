from conans import ConanFile, CMake, MSBuild, tools


class FreeimageConan(ConanFile):
    name = "freeimage"
    version = "3.18.0"
    license = "GPLv2, GPLv3, FIPL"
    author = "<Put your name here> <And your email here>"
    url = "https://freeimage.sourceforge.io/"
    description = "<Description of Freeimage here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    short_paths = True
    generators = "cmake"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def source(self):
        source_url = "http://downloads.sourceforge.net/freeimage"
        tools.get("{0}/FreeImage{1}.zip".format(source_url, self.version.replace('.', '')))
        extracted_dir = self.name + "-" + self.version

        #Rename to "source_subfolder" is a convention to simplify later steps
        # os.rename(extracted_dir, self.source_subfolder)

        # self.run("git clone https://github.com/conan-io/hello.git")
        # This small hack might be useful to guarantee proper /MT /MD linkage
        # in MSVC if the packaged project doesn't have variables to set it
        # properly
#         tools.replace_in_file("hello/CMakeLists.txt", "PROJECT(HelloWorld)",
#                               '''PROJECT(HelloWorld)
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()''')

    def build(self):
        msbuild = MSBuild(self)
        msbuild.build("FreeImage/FreeImage.2013.sln", upgrade_project=False, use_env=False)
        # cmake = CMake(self)
        # cmake.configure(source_folder="freeimage")
        # cmake.build()

        # Explicit way:
        # self.run('cmake %s/freeimage %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="FreeImage/Dist", keep_path=False)
        self.copy("*freeimage.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["freeimage"]

