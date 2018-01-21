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

project (ice_engine)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ICEENGINE_COMPILER_FLAGS "-DBOOST_LOG_DYN_LINK")
set(ICEENGINE_LINKER_FLAGS "-no-pie")
set(BOOST_VERSION "1.66.0")
set(BOOST_FILENAME_PREFIX "lib")
set(BOOST_FILENAME_POSTFIX "")
if(MSVC)
  set(ICEENGINE_COMPILER_FLAGS "/EHsc /MP")
  set(ICEENGINE_LINKER_FLAGS "")
  if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(ICEENGINE_COMPILER_FLAGS "/EHsc /MP")
    set(ICEENGINE_LINKER_FLAGS "")
  endif()
  set(BOOST_FILENAME_PREFIX "lib")
  set(BOOST_FILENAME_POSTFIX "-vc141-mt-1_66")
endif()

set(ICEENGINE_COMPILER_FLAGS "${ICEENGINE_COMPILER_FLAGS} -DAS_USE_STLNAMES")

# Custom options
option(ICEENGINE_BUILD_AS_LIBRARY "ICEENGINE_BUILD_AS_LIBRARY" FALSE)
option(ICEENGINE_BUILD_TESTS "ICEENGINE_BUILD_TESTS" FALSE)
option(ICEENGINE_BUILD_BENCHMARKS "ICEENGINE_BUILD_BENCHMARKS" FALSE)
option(ICEENGINE_ENABLE_DEBUG_LOGGING "ICEENGINE_ENABLE_DEBUG_LOGGING" FALSE)

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo OR ICEENGINE_ENABLE_DEBUG_LOGGING)
  set(ICEENGINE_COMPILER_FLAGS "${ICEENGINE_COMPILER_FLAGS} -DICEENGINE_ENABLE_DEBUG_LOGGING")
endif()

if(NOT DEFINED ICEENGINE_GRAPHICS_BACKEND)
  #option(ICEENGINE_GRAPHICS_BACKEND "ICEENGINE_GRAPHICS_BACKEND" "noop")
  set(ICEENGINE_GRAPHICS_BACKEND "opengl")
  set(ICEENGINE_GRAPHICS_BACKEND "opengl" PARENT_SCOPE)
endif()

# Dependencies
set(PROJECT_LINK_LIBRARY_DIRECTORIES
  ./deps/angelscript/lib
  ./deps/recastnavigation/lib
  ./deps/freeimage/lib
  ./deps/sqlite/lib
  ./deps/boost/lib
  ./deps/assimp/lib
  ./deps/entityx/lib
  ./deps/bullet/lib
  ./deps/sdl/lib
  ./deps/glew/lib
  ./deps/yojimbo/lib
  ./deps/yojimbo/lib/libsodium
  ./deps/yojimbo/lib/mbedtls
  ./deps/openalsoft/lib
)

find_library(SQLITE_LIBRARY NAMES sqlite3.a sqlite3.so sqlite3.lib sqlite3.dll PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
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
find_library(YOJIMBO_LIBRARY NAMES yojimbo PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(LIBSODIUM_LIBRARY NAMES sodium PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(MBEDTLS_LIBRARY NAMES mbedtls PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(MBEDCRYPTO_LIBRARY NAMES mbedcrypto PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(MBEDX509_LIBRARY NAMES mbedx509 PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(OPENALSOFT_LIBRARY NAMES openal OpenAL32 PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(SDL_LIBRARY NAMES SDL2 PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(SDL_MAIN_LIBRARY NAMES SDL2main PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(ANGELSCRIPT_LIBRARY NAMES angelscript PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(RECASTNAVIGATION_DETOUR_LIBRARY NAMES Detour PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(RECASTNAVIGATION_DETOUR_CROWD_LIBRARY NAMES DetourCrowd PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(RECASTNAVIGATION_DETOUR_TILE_CACHE_LIBRARY NAMES DetourTileCache PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(RECASTNAVIGATION_RECAST_LIBRARY NAMES Recast PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
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
include_directories(./deps/yojimbo/include)
include_directories(./deps/yojimbo/include/libsodium)
include_directories(./deps/yojimbo/include/mbedtls)
include_directories(./deps/openalsoft/include)
include_directories(./deps/assimp/include)
include_directories(./deps/boost/include)
include_directories(./deps/angelscript/include)
include_directories(./deps/angelscript/include/angelscript)
include_directories(./deps/recastnavigation/include)
include_directories(./deps/recastnavigation/include/Detour)
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
  configure_file(deps/freeimage/lib/FreeImage.dll ./ COPYONLY)
  configure_file(deps/glew/lib/glew32.dll ./ COPYONLY)
  configure_file(deps/assimp/lib/assimp.dll ./ COPYONLY)
  configure_file(deps/openalsoft/lib/OpenAL32.dll ./ COPYONLY)
else()
  configure_file(${FREEIMAGE_LIBRARY} ./libfreeimage.so.3 COPYONLY)
  configure_file(${LIBSODIUM_LIBRARY} ./libsodium.so.23 COPYONLY)
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
add_definitions(${ICEENGINE_COMPILER_FLAGS})


# Generate library
if(ICEENGINE_BUILD_AS_LIBRARY OR BUILD_SHARED_LIBS)
  list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")
  add_library(ice_engine ${SOURCES})
else()
  add_executable(ice_engine ${SOURCES})
endif()

target_link_libraries(ice_engine ${SQLITE_LIBRARY} )
target_link_libraries(ice_engine ${FREEIMAGE_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_SYSTEM_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_CHRONO_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_ATOMIC_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_PROGRAM_OPTIONS_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_FILESYSTEM_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_THREAD_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_LOG_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_LOG_SETUP_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_DATE_TIME_LIBRARY} )
target_link_libraries(ice_engine ${BOOST_REGEX_LIBRARY} )
target_link_libraries(ice_engine ${ASSIMP_LIBRARY} )
target_link_libraries(ice_engine ${GLEW_LIBRARY} )
target_link_libraries(ice_engine ${YOJIMBO_LIBRARY} )
target_link_libraries(ice_engine ${LIBSODIUM_LIBRARY} )
target_link_libraries(ice_engine ${MBEDTLS_LIBRARY} )
target_link_libraries(ice_engine ${MBEDX509_LIBRARY} )
target_link_libraries(ice_engine ${MBEDCRYPTO_LIBRARY} )
target_link_libraries(ice_engine ${OPENALSOFT_LIBRARY} )
target_link_libraries(ice_engine ${SDL_LIBRARY} )
target_link_libraries(ice_engine ${SDL_MAIN_LIBRARY} )
target_link_libraries(ice_engine ${ANGELSCRIPT_LIBRARY} )
target_link_libraries(ice_engine ${RECASTNAVIGATION_DETOUR_LIBRARY} )
target_link_libraries(ice_engine ${RECASTNAVIGATION_DETOUR_CROWD_LIBRARY} )
target_link_libraries(ice_engine ${RECASTNAVIGATION_DETOUR_TILE_CACHE_LIBRARY} )
target_link_libraries(ice_engine ${RECASTNAVIGATION_RECAST_LIBRARY} )
target_link_libraries(ice_engine ${ENTITYX_LIBRARY} )
target_link_libraries(ice_engine ${BULLET_COMMON_LIBRARY} )
target_link_libraries(ice_engine ${BULLET_DYNAMICS_LIBRARY} )
target_link_libraries(ice_engine ${BULLET_COLLISION_LIBRARY} )
target_link_libraries(ice_engine ${BULLET_LINEAR_MATH_LIBRARY} )

target_link_libraries(ice_engine Threads::Threads)
target_link_libraries(ice_engine ${OPENGL_gl_LIBRARY})
target_link_libraries(ice_engine ${X11_LIBRARIES})
target_link_libraries(ice_engine ${X11_Xext_LIB})
target_link_libraries(ice_engine ${X11_Xxf86vm_LIB})
target_link_libraries(ice_engine ${CMAKE_DL_LIBS})

if(WIN32)
  target_link_libraries(ice_engine winmm)
  target_link_libraries(ice_engine imm32)
  target_link_libraries(ice_engine version)
  target_link_libraries(ice_engine Psapi)
endif()

if (ICEENGINE_BUILD_TESTS)
  enable_testing()
  # Test project dependency
  add_subdirectory(tests)
endif()

if (ICEENGINE_BUILD_BENCHMARKS)
  enable_testing()
  # Test project dependency
  add_subdirectory(benchmarks)
endif()

# Disable Position Independent Executable - this is starting to be enabled by default,
# which is causing issues
target_link_libraries(ice_engine ${ICEENGINE_LINKER_FLAGS})

# Set install dir
install(TARGETS ice_engine DESTINATION /usr/lib)
