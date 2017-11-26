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

project(hercules_benchmarks)

option(HERCULES_BENCHMARKS_EXPORT_TO_CSV "HERCULES_BENCHMARKS_EXPORT_TO_CSV" FALSE)

list(APPEND PROJECT_LINK_LIBRARY_DIRECTORIES ../deps/celero/lib)

find_library(CELERO_LIBRARY NAMES celero_static celero PATHS ${PROJECT_LINK_LIBRARY_DIRECTORIES})

include_directories(../include)
include_directories(../deps/celero/include)

if(WIN32)
  configure_file(../deps/freeimage/lib/FreeImage.dll ./ COPYONLY)
  configure_file(../deps/glew/lib/glew32.dll ./ COPYONLY)
  configure_file(../deps/assimp/lib/assimp.dll ./ COPYONLY)
else()
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

# Flags
add_definitions(-DCELERO_STATIC)

macro(create_benchmark BENCHMARK_NAME EXECUTABLE_NAME SOURCE)
  add_executable(${EXECUTABLE_NAME} "src/${SOURCE}")
  add_dependencies(${EXECUTABLE_NAME} hercules)

  if(MSVC)
    target_link_libraries(${EXECUTABLE_NAME} ../${CMAKE_BUILD_TYPE}/hercules)
  else()
    target_link_libraries(${EXECUTABLE_NAME} hercules)
  endif()
  
  target_link_libraries(${EXECUTABLE_NAME} ${SQLITE_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_SYSTEM_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_CHRONO_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_ATOMIC_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_PROGRAM_OPTIONS_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_FILESYSTEM_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_THREAD_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_LOG_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_LOG_SETUP_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_DATE_TIME_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${CELERO_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BOOST_REGEX_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${ASSIMP_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${SDL_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${SDL_MAIN_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${ANGELSCRIPT_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${ENTITYX_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BULLET_COMMON_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BULLET_DYNAMICS_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BULLET_COLLISION_LIBRARY} )
  target_link_libraries(${EXECUTABLE_NAME} ${BULLET_LINEAR_MATH_LIBRARY} )
  
  target_link_libraries(${EXECUTABLE_NAME} Threads::Threads)
  target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
  target_link_libraries(${EXECUTABLE_NAME} ${X11_LIBRARIES})
  target_link_libraries(${EXECUTABLE_NAME} ${X11_Xext_LIB})
  target_link_libraries(${EXECUTABLE_NAME} ${X11_Xxf86vm_LIB})
  target_link_libraries(${EXECUTABLE_NAME} ${CMAKE_DL_LIBS})
  
  if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME} winmm)
    target_link_libraries(${EXECUTABLE_NAME} imm32)
    target_link_libraries(${EXECUTABLE_NAME} version)
    target_link_libraries(${EXECUTABLE_NAME} Psapi)
    target_link_libraries(${EXECUTABLE_NAME} PowrProf)
  endif()
  
  if(HERCULES_BENCHMARKS_EXPORT_TO_CSV)
    add_test(${BENCHMARK_NAME} ${EXECUTABLE_NAME} -t ${BENCHMARK_NAME}.csv)
  else()
    add_test(${BENCHMARK_NAME} ${EXECUTABLE_NAME})
  endif()
  
  
endmacro()

create_benchmark(ScriptingEngineBenchmarks ScriptingEngineBenchmarks ScriptingEngine.cpp)