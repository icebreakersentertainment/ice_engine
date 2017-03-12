cmake_minimum_required(VERSION 3.1.0)

IF(DEFINED CMAKE_BUILD_TYPE)
  set(
    CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
  )
else()
  set(
    CMAKE_BUILD_TYPE Release CACHE STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
  )
endif()

project (hercules)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(HERCULES_COMPILER_FLAGS "-DBOOST_LOG_DYN_LINK")
set(HERCULES_LINKER_FLAGS "-no-pie")
if(CMAKE_BUILD_TYPE MATCHES DEBUG)
  set(HERCULES_COMPILER_FLAGS "-DBOOST_LOG_DYN_LINK")
  set(HERCULES_LINKER_FLAGS "-no-pie")
endif()
set(BOOST_VERSION "1.63.0")
set(BOOST_FILENAME_PREFIX "lib")
set(BOOST_FILENAME_POSTFIX "")
if(MSVC)
  set(HERCULES_COMPILER_FLAGS "/EHsc /MP")
  set(HERCULES_LINKER_FLAGS "")
  if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(HERCULES_COMPILER_FLAGS "/EHsc /MP")
    set(HERCULES_LINKER_FLAGS "")
  endif()
  set(BOOST_FILENAME_PREFIX "lib")
  set(BOOST_FILENAME_POSTFIX "-vc140-mt-1_63")
endif()

# Custom options
option(HERCULES_BUILD_AS_LIBRARY "HERCULES_BUILD_AS_LIBRARY" FALSE)
option(HERCULES_BUILD_TESTS "HERCULES_BUILD_TESTS" FALSE)

if(NOT DEFINED HERCULES_GRAPHICS_BACKEND)
  #option(HERCULES_GRAPHICS_BACKEND "HERCULES_GRAPHICS_BACKEND" "noop")
  if(WIN32)
    set(HERCULES_GRAPHICS_BACKEND "directx")
  else()
    set(HERCULES_GRAPHICS_BACKEND "opengl")
  endif()
endif()

# Dependencies
set(PROJECT_LINK_LIBRARY_DIRECTORIES
  ./deps/bgfx/lib/${HERCULES_GRAPHICS_BACKEND}
  ./deps/bx/lib
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

find_library(SQLITE_LIBRARY NAMES sqlite3.a sqlite3.so sqlite3.lib sqlite3.dll PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BGFX_LIBRARY NAMES bgfxRelease bgfx-shared-libRelease PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BX_LIBRARY NAMES bxRelease bx-shared-libRelease PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(FREEIMAGE_LIBRARY NAMES freeimage-3.17.0 FreeImage PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BOOST_SYSTEM_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_system${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_system${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(BOOST_CHRONO_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_chrono${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_chrono${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(BOOST_ATOMIC_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_atomic${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_atomic${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(BOOST_PROGRAM_OPTIONS_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_program_options${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_program_options${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(BOOST_FILESYSTEM_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_filesystem${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_filesystem${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(BOOST_THREAD_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_thread${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_thread${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(BOOST_LOG_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_log${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_log${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(BOOST_LOG_SETUP_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_log_setup${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_log_setup${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(BOOST_DATE_TIME_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_date_time${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_date_time${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(BOOST_REGEX_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_regex${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_regex${BOOST_FILENAME_POSTFIX}
  PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES}
)
find_library(ASSIMP_LIBRARY NAMES libassimp.so.3 assimp PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(GLEW_LIBRARY NAMES libGLEW.so.2.0 glew32 PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(SDL_LIBRARY NAMES SDL2 PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(SDL_MAIN_LIBRARY NAMES SDL2main PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(ANGELSCRIPT_LIBRARY NAMES angelscript PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(ENTITYX_LIBRARY NAMES libentityx.so.1 entityx PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BULLET_COMMON_LIBRARY NAMES Bullet3Common PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BULLET_DYNAMICS_LIBRARY NAMES BulletDynamics PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BULLET_COLLISION_LIBRARY NAMES BulletCollision PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(BULLET_LINEAR_MATH_LIBRARY NAMES LinearMath PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

include_directories(./deps/glm/include)
include_directories(./deps/glew/include)
include_directories(./deps/assimp/include)
include_directories(./deps/boost/include)
include_directories(./deps/angelscript/include)
include_directories(./deps/angelscript/include/angelscript)
include_directories(./deps/bgfx/include)
include_directories(./deps/bx/include)
include_directories(./deps/bullet/include)
include_directories(./deps/bullet/include/bullet)
include_directories(./deps/entityx/include)
include_directories(./deps/sdl/include)
include_directories(./deps/sqlite/include)
include_directories(./deps/ctpl/include)
include_directories(./deps/freeimage/include)

if(MSVC)
  include_directories(./deps/bx/include/compat/msvc)
endif()

if(WIN32)
  configure_file(${FREEIMAGE_LIBRARY} ./ COPYONLY)
  configure_file(${ASSIMP_LIBRARY} ./ COPYONLY)
  configure_file(${GLEW_LIBRARY} ./ COPYONLY)
else()
  configure_file(${BGFX_LIBRARY} ./ COPYONLY)
  configure_file(${BX_LIBRARY} ./ COPYONLY)
  configure_file(${FREEIMAGE_LIBRARY} ./libfreeimage.so.3 COPYONLY)
  configure_file(${BOOST_SYSTEM_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_CHRONO_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_ATOMIC_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_PROGRAM_OPTIONS_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_FILESYSTEM_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_THREAD_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_LOG_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_LOG_SETUP_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_DATE_TIME_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_REGEX_LIBRARY} ./ COPYONLY)
  configure_file(${ASSIMP_LIBRARY} ./ COPYONLY)
  configure_file(${GLEW_LIBRARY} ./ COPYONLY)
  configure_file(${ENTITYX_LIBRARY} ./ COPYONLY)
endif()

# Set our runtime library locations
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")

# Headers
include_directories(include)

# Source
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Flags
#set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${HERCULES_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${HERCULES_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} ${HERCULES_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${HERCULES_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} ${HERCULES_COMPILER_FLAGS}")


# Generate library
if(BUILD_AS_LIBRARY OR BUILD_SHARED_LIBS)
  list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")
  add_library(hercules ${SOURCES})
else()
  add_executable(hercules ${SOURCES})
endif()

target_link_libraries(hercules ${SQLITE_LIBRARY} )
target_link_libraries(hercules ${BGFX_LIBRARY} )
target_link_libraries(hercules ${BX_LIBRARY} )
target_link_libraries(hercules ${FREEIMAGE_LIBRARY} )
target_link_libraries(hercules ${BOOST_SYSTEM_LIBRARY} )
target_link_libraries(hercules ${BOOST_CHRONO_LIBRARY} )
target_link_libraries(hercules ${BOOST_ATOMIC_LIBRARY} )
target_link_libraries(hercules ${BOOST_PROGRAM_OPTIONS_LIBRARY} )
target_link_libraries(hercules ${BOOST_FILESYSTEM_LIBRARY} )
target_link_libraries(hercules ${BOOST_THREAD_LIBRARY} )
target_link_libraries(hercules ${BOOST_LOG_LIBRARY} )
target_link_libraries(hercules ${BOOST_LOG_SETUP_LIBRARY} )
target_link_libraries(hercules ${BOOST_DATE_TIME_LIBRARY} )
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
target_link_libraries(hercules ${X11_LIBRARIES})
target_link_libraries(hercules ${X11_Xext_LIB})
target_link_libraries(hercules ${X11_Xxf86vm_LIB})
target_link_libraries(hercules ${CMAKE_DL_LIBS})

if(WIN32)
  target_link_libraries(hercules winmm)
  target_link_libraries(hercules imm32)
  target_link_libraries(hercules version)
  target_link_libraries(hercules Psapi)
endif()

if (HERCULES_BUILD_TESTS)
  enable_testing()
  # Test project dependency
  add_subdirectory(tests)
endif()

# Disable Position Independent Executable - this is starting to be enabled by default,
# which is causing issues
target_link_libraries(hercules ${HERCULES_LINKER_FLAGS})

# Set install dir
install(TARGETS hercules DESTINATION /usr/lib)
