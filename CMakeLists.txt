cmake_minimum_required(VERSION 3.1.0)

project (ice_engine)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(ICEENGINE_COMPILER_FLAGS "")
set(ICEENGINE_DEFINITIONS -DAS_USE_STLNAMES)
set(ICEENGINE_LINKER_FLAGS "-no-pie")
if(MSVC)
  set(ICEENGINE_COMPILER_FLAGS "/EHsc /MP")
  set(ICEENGINE_LINKER_FLAGS "")
  if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(ICEENGINE_COMPILER_FLAGS "/EHsc /MP")
    set(ICEENGINE_LINKER_FLAGS "")
  endif()
elseif(UNIX AND NOT APPLE)
  list(APPEND ICEENGINE_DEFINITIONS -DBOOST_ALL_DYN_LINK)
elseif(APPLE)
  set(ICEENGINE_LINKER_FLAGS "")
  list(APPEND ICEENGINE_DEFINITIONS -DBOOST_ALL_DYN_LINK)
endif()

# Custom options
option(ICEENGINE_BUILD_AS_LIBRARY "ICEENGINE_BUILD_AS_LIBRARY" FALSE)
option(ICEENGINE_BUILD_TESTS "ICEENGINE_BUILD_TESTS" FALSE)
option(ICEENGINE_BUILD_BENCHMARKS "ICEENGINE_BUILD_BENCHMARKS" FALSE)
option(ICEENGINE_ENABLE_DEBUG_LOGGING "ICEENGINE_ENABLE_DEBUG_LOGGING" FALSE)
option(ICEENGINE_ENABLE_TRACE_LOGGING "ICEENGINE_ENABLE_TRACE_LOGGING" FALSE)

if(CMAKE_BUILD_TYPE MATCHES Debug OR CMAKE_BUILD_TYPE MATCHES RelWithDebInfo OR ICEENGINE_ENABLE_DEBUG_LOGGING)
  list(APPEND ICEENGINE_DEFINITIONS -DICEENGINE_ENABLE_DEBUG_LOGGING)
endif()

if(ICEENGINE_ENABLE_TRACE_LOGGING)
  list(APPEND ICEENGINE_DEFINITIONS -DICEENGINE_ENABLE_TRACE_LOGGING)
endif()

# Dependencies
set(PROJECT_LINK_LIBRARY_DIRECTORIES
  ./deps/angelscript/lib
  ./deps/freeimage/lib
  ./deps/boost/lib
  ./deps/assimp/lib
  ./deps/entityx/lib
  ./deps/sdl/lib
)

include(cmake/BoostConfig.cmake)
include(cmake/AngelscriptConfig.cmake)
include(cmake/FreeImageConfig.cmake)
set(SDL2_BUILDING_LIBRARY TRUE)
include(cmake/SDL2Config.cmake)
include(cmake/GlmConfig.cmake)
include(cmake/GlewConfig.cmake)

find_library(ASSIMP_LIBRARY NAMES libassimp.so.3 libassimp.3.dylib assimp-vc140-mt PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
#find_library(SDL_LIBRARY NAMES SDL2 PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
#find_library(SDL_MAIN_LIBRARY NAMES SDL2main PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
#find_library(ANGELSCRIPT_LIBRARY NAMES angelscript PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})
find_library(ENTITYX_LIBRARY NAMES libentityx.so.1 libentityx.1.dylib entityx PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

if(UNIX AND NOT APPLE)
  find_package(X11 REQUIRED)
elseif(APPLE)  
  find_library(QUARTZCORE_LIBRARY QuartzCore REQUIRED)
  find_library(METAL_LIBRARY Metal REQUIRED)
  find_library(APPKIT_LIBRARY AppKit REQUIRED)
  find_library(APPSERV_LIBRARY ApplicationServices REQUIRED)
  find_library(CARBON_LIBRARY Carbon REQUIRED)
  find_library(COREAUDIO_LIBRARY CoreAudio REQUIRED)
  find_library(FORCEFEEDBACK_LIBRARY ForceFeedback REQUIRED)
  find_library(IOK_LIBRARY IOKit REQUIRED)
  find_library(AUDIOTOOLBOX_LIBRARY AudioToolbox REQUIRED)
  find_library(ICONV_LIBRARY iconv REQUIRED)
endif()

if(WIN32)
  configure_file(deps/freeimage/lib/FreeImage.dll ./ COPYONLY)
  configure_file(deps/assimp/lib/assimp-vc140-mt.dll ./ COPYONLY)
  configure_file(deps/sdl/lib/SDL2.dll ./ COPYONLY)
else()
  configure_file(${FREEIMAGE_LIBRARY} ./libfreeimage.so.3 COPYONLY)
  configure_file(${BOOST_SYSTEM_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_CHRONO_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_ATOMIC_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_THREAD_LIBRARY_FILENAME} ./ COPYONLY)
  configure_file(${BOOST_PROGRAM_OPTIONS_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_FILESYSTEM_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_FILESYSTEM_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_LOG_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_LOG_SETUP_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_DATE_TIME_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_REGEX_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_SERIALIZATION_LIBRARY} ./ COPYONLY)
  configure_file(${ASSIMP_LIBRARY} ./ COPYONLY)
  configure_file(${ENTITYX_LIBRARY} ./ COPYONLY)
endif()

set(ICEENGINE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" CACHE STRING "Where the Ice Engine include directory can be found")

# Set our runtime library locations
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "$ORIGIN")
if(APPLE)
  set(CMAKE_INSTALL_RPATH "./")
  set(CMAKE_MACOSX_RPATH ON)
endif()

# Source
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Generate library
if(ICEENGINE_BUILD_AS_LIBRARY)
  list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")
  add_library(ice_engine STATIC ${SOURCES})
else()
  add_executable(ice_engine ${SOURCES})
endif()

# Headers
target_include_directories(ice_engine PRIVATE include)

target_include_directories(ice_engine PRIVATE ${GLM_INCLUDE_DIRS})
target_include_directories(ice_engine PRIVATE ./deps/assimp/include)
target_include_directories(ice_engine PRIVATE ${BOOST_INCLUDE_DIRS})
target_include_directories(ice_engine PRIVATE ${ANGELSCRIPT_INCLUDE_DIRS})
target_include_directories(ice_engine PRIVATE ./deps/entityx/include)
target_include_directories(ice_engine PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(ice_engine PRIVATE ./deps/ctpl/include)
target_include_directories(ice_engine PRIVATE ./deps/freeimage/include)

target_compile_definitions(ice_engine PRIVATE ${ICEENGINE_DEFINITIONS})
target_compile_definitions(ice_engine PRIVATE ICEENGINE_ENABLE_TRACE_LOGGING=1)
target_compile_options(ice_engine PRIVATE ${ICEENGINE_COMPILER_FLAGS})

target_link_libraries(ice_engine ${FREEIMAGE_LIBRARIES} )
target_link_libraries(ice_engine ${Boost_SYSTEM_LIBRARY} )
target_link_libraries(ice_engine ${Boost_CHRONO_LIBRARY} )
target_link_libraries(ice_engine ${Boost_ATOMIC_LIBRARY} )
target_link_libraries(ice_engine ${Boost_THREAD_LIBRARY} )
target_link_libraries(ice_engine ${Boost_PROGRAM_OPTIONS_LIBRARY} )
target_link_libraries(ice_engine ${Boost_FILESYSTEM_LIBRARY} )
target_link_libraries(ice_engine ${Boost_LOG_LIBRARY} )
target_link_libraries(ice_engine ${Boost_LOG_SETUP_LIBRARY} )
target_link_libraries(ice_engine ${Boost_DATE_TIME_LIBRARY} )
target_link_libraries(ice_engine ${Boost_REGEX_LIBRARY} )
target_link_libraries(ice_engine ${Boost_SERIALIZATION_LIBRARY} )
target_link_libraries(ice_engine ${ASSIMP_LIBRARY} )
target_link_libraries(ice_engine ${SDL2_LIBRARIES} )
target_link_libraries(ice_engine ${ANGELSCRIPT_LIBRARY} )
target_link_libraries(ice_engine ${ENTITYX_LIBRARY} )

target_link_libraries(ice_engine Threads::Threads)
target_link_libraries(ice_engine ${OPENGL_gl_LIBRARY})
if(UNIX AND NOT APPLE)
  target_link_libraries(ice_engine ${X11_LIBRARIES})
  target_link_libraries(ice_engine ${X11_Xext_LIB})
  target_link_libraries(ice_engine ${X11_Xxf86vm_LIB})
elseif(APPLE)
  target_link_libraries(ice_engine ${QUARTZCORE_LIBRARY})
  target_link_libraries(ice_engine ${METAL_LIBRARY})
  target_link_libraries(ice_engine ${APPKIT_LIBRARY})
  target_link_libraries(ice_engine ${APPSERV_LIBRARY})
  target_link_libraries(ice_engine ${CARBON_LIBRARY})
  target_link_libraries(ice_engine ${COREAUDIO_LIBRARY})
  target_link_libraries(ice_engine ${FORCEFEEDBACK_LIBRARY})
  target_link_libraries(ice_engine ${IOK_LIBRARY})
  target_link_libraries(ice_engine ${AUDIOTOOLBOX_LIBRARY})
  target_link_libraries(ice_engine ${ICONV_LIBRARY})
endif()

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
