cmake_minimum_required(VERSION 3.1.0)
project (hercules)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BOOST_VERSION "1.63.0")

# Custom options
option(BUILD_AS_LIBRARY "BUILD_AS_LIBRARY" FALSE)

# Dependencies
set(PROJECT_LINK_LIBRARY_DIRECTORIES
  ./deps/bgfx/lib
  ./deps/angelscript/lib
  ./deps/freeimage/lib
  ./deps/sqlite/lib
  ./deps/boost/lib
  ./deps/assimp/lib
  ./deps/entityx/lib
  ./deps/bullet/lib
  ./deps/sdl/lib
  ./deps/glew/lib
)

find_library(SQLITE_LIBRARY NAMES sqlite3.a sqlite3.so PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BGFX_LIBRARY NAMES libbgfx-shared-libRelease.so PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(FREEIMAGE_LIBRARY NAMES libfreeimage-3.17.0.so PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BOOST_SYSTEM_LIBRARY NAMES libboost_system.so.${BOOST_VERSION} PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BOOST_PROGRAM_OPTIONS_LIBRARY NAMES libboost_program_options.so.${BOOST_VERSION} PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BOOST_FILESYSTEM_LIBRARY NAMES libboost_filesystem.so.${BOOST_VERSION} PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BOOST_THREAD_LIBRARY NAMES libboost_thread.so.${BOOST_VERSION} PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BOOST_LOG_LIBRARY NAMES libboost_log.so.${BOOST_VERSION} PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BOOST_LOG_SETUP_LIBRARY NAMES libboost_log_setup.so.${BOOST_VERSION} PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BOOST_REGEX_LIBRARY NAMES libboost_regex.so.${BOOST_VERSION} PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BOOST_UNIT_TEST_FRAMEWORK_LIBRARY NAMES libboost_unit_test_framework.so.${BOOST_VERSION} PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(ASSIMP_LIBRARY NAMES libassimp.so.3 PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(GLEW_LIBRARY NAMES libGLEW.so.2.0 PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(SDL_LIBRARY NAMES libSDL2.a PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(SDL_MAIN_LIBRARY NAMES libSDL2main.a PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(ANGELSCRIPT_LIBRARY NAMES libangelscript.a PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(ENTITYX_LIBRARY NAMES libentityx.so.1 PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BULLET_COMMON_LIBRARY NAMES libBullet3Common.a PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BULLET_DYNAMICS_LIBRARY NAMES libBulletDynamics.a PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BULLET_COLLISION_LIBRARY NAMES libBulletCollision.a PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BULLET_LINEAR_MATH_LIBRARY NAMES libLinearMath.a PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(./deps/glm/include)
include_directories(./deps/glew/include)
include_directories(./deps/assimp/include)
include_directories(./deps/boost/include)
include_directories(./deps/angelscript/include)
include_directories(./deps/angelscript/include/angelscript)
include_directories(./deps/bgfx/include)
include_directories(./deps/bullet/include)
include_directories(./deps/bullet/include/bullet)
include_directories(./deps/entityx/include)
include_directories(./deps/sdl/include)
include_directories(./deps/sqlite/include)
include_directories(./deps/ctpl/include)
include_directories(./deps/freeimage/include)

configure_file(${BGFX_LIBRARY} ./ COPYONLY)
configure_file(${FREEIMAGE_LIBRARY} libfreeimage.so.3 COPYONLY)
configure_file(${BOOST_SYSTEM_LIBRARY} ./ COPYONLY)
configure_file(${BOOST_PROGRAM_OPTIONS_LIBRARY} ./ COPYONLY)
configure_file(${BOOST_FILESYSTEM_LIBRARY} ./ COPYONLY)
configure_file(${BOOST_THREAD_LIBRARY} ./ COPYONLY)
configure_file(${BOOST_LOG_LIBRARY} ./ COPYONLY)
configure_file(${BOOST_LOG_SETUP_LIBRARY} ./ COPYONLY)
configure_file(${BOOST_REGEX_LIBRARY} ./ COPYONLY)
configure_file(${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY} ./ COPYONLY)
configure_file(${ASSIMP_LIBRARY} ./ COPYONLY)
configure_file(${GLEW_LIBRARY} ./ COPYONLY)
configure_file(${ENTITYX_LIBRARY} ./ COPYONLY)

# Set our runtime library locations
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")

# Headers
include_directories(include)

# Source
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Flags
#set(CMAKE_CXX_FLAGS "-Wall")
#set(CMAKE_CXX_FLAGS "-DBOOST_LOG_DYN_LINK")
set(SHARED_COMPILE_FLAGS
  "-DBOOST_LOG_DYN_LINK"
)

# Generate library
if(BUILD_AS_LIBRARY OR BUILD_SHARED_LIBS)
  add_library(hercules ${SOURCES})
else()
  add_executable(hercules ${SOURCES})
endif()

target_link_libraries(hercules ${SQLITE_LIBRARY} )
target_link_libraries(hercules ${BGFX_LIBRARY} )
target_link_libraries(hercules ${FREEIMAGE_LIBRARY} )
target_link_libraries(hercules ${BOOST_SYSTEM_LIBRARY} )
target_link_libraries(hercules ${BOOST_PROGRAM_OPTIONS_LIBRARY} )
target_link_libraries(hercules ${BOOST_FILESYSTEM_LIBRARY} )
target_link_libraries(hercules ${BOOST_THREAD_LIBRARY} )
target_link_libraries(hercules ${BOOST_LOG_LIBRARY} )
target_link_libraries(hercules ${BOOST_LOG_SETUP_LIBRARY} )
target_link_libraries(hercules ${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY} )
target_link_libraries(hercules ${BOOST_REGEX_LIBRARY} )
target_link_libraries(hercules ${ASSIMP_LIBRARY} )
target_link_libraries(hercules ${GLEW_LIBRARY} )
target_link_libraries(hercules ${SDL_LIBRARY} )
target_link_libraries(hercules ${SDL_MAIN_LIBRARY} )
target_link_libraries(hercules ${ANGELSCRIPT_LIBRARY} )
target_link_libraries(hercules ${ENTITYX_LIBRARY} )
target_link_libraries(hercules ${BULLET_COMMON_LIBRARY} )
target_link_libraries(hercules ${BULLET_DYNAMICS_LIBRARY} )
target_link_libraries(hercules ${BULLET_COLLISION_LIBRARY} )
target_link_libraries(hercules ${BULLET_LINEAR_MATH_LIBRARY} )

target_link_libraries(hercules Threads::Threads)
target_link_libraries(hercules ${OPENGL_gl_LIBRARY})
target_link_libraries(hercules ${CMAKE_DL_LIBS})

set_target_properties(hercules PROPERTIES
  COMPILE_FLAGS ${SHARED_COMPILE_FLAGS}
)

# Set install dir
install(TARGETS hercules DESTINATION /usr/lib)
