cmake_minimum_required(VERSION 3.1.0)

project(ice_engine_benchmarks)

option(ICEENGINE_BENCHMARKS_EXPORT_TO_CSV "ICEENGINE_BENCHMARKS_EXPORT_TO_CSV" FALSE)

find_package(freeimage REQUIRED)
find_package(angelscript REQUIRED)
find_package(entityx REQUIRED)
find_package(ctpl REQUIRED)
find_package(celero REQUIRED)
find_package(Boost REQUIRED)

if(WIN32)
  configure_file(${freeimage_LIB_DIRS}/../bin/FreeImage.dll ./ COPYONLY)
elseif(UNIX)
  configure_file(${FREEIMAGE_LIBRARY} ./libfreeimage.so.3 COPYONLY)
  configure_file(${BOOST_SYSTEM_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_CHRONO_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_ATOMIC_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_PROGRAM_OPTIONS_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_FILESYSTEM_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_THREAD_LIBRARY_FILENAME} ./ COPYONLY)
  configure_file(${BOOST_LOG_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_LOG_SETUP_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_DATE_TIME_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_REGEX_LIBRARY} ./ COPYONLY)
  configure_file(${ASSIMP_LIBRARY} ./ COPYONLY)
  configure_file(${ENTITYX_LIBRARY} ./ COPYONLY)
endif()

if(APPLE)
  configure_file(${BOOST_SERIALIZATION_LIBRARY} ./ COPYONLY)
endif()

macro(create_benchmark BENCHMARK_NAME EXECUTABLE_NAME SOURCE)
  add_executable(${EXECUTABLE_NAME} "src/${SOURCE}")

  target_include_directories(${EXECUTABLE_NAME} PRIVATE ${ICEENGINE_INCLUDE_DIRS})
  target_include_directories(${EXECUTABLE_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
#  target_include_directories(${EXECUTABLE_NAME} PRIVATE ../deps/celero/include)

  target_compile_options(${EXECUTABLE_NAME} PRIVATE -DCELERO_STATIC)
  
  add_dependencies(${EXECUTABLE_NAME} ice_engine)

  target_link_libraries(${EXECUTABLE_NAME} PRIVATE ice_engine)

#  if(MSVC)
#    target_link_libraries(${EXECUTABLE_NAME} ../${CMAKE_BUILD_TYPE}/ice_engine)
#  else()
#    target_link_libraries(${EXECUTABLE_NAME} ice_engine)
#  endif()
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE PRIVATE angelscript::angelscript)
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE PRIVATE entityx::entityx)
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE PRIVATE ctpl::ctpl)
  target_link_libraries(${EXECUTABLE_NAME} PRIVATE PRIVATE celero::celero)

  if(UNIX AND NOT APPLE)
      target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${X11_LIBRARIES})
      target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${X11_Xext_LIB})
      target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${X11_Xxf86vm_LIB})
      target_link_libraries(${EXECUTABLE_NAME} PUBLIC Threads::Threads)
      target_link_libraries(${EXECUTABLE_NAME} PUBLIC ${CMAKE_DL_LIBS})
  endif()
  
  if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE winmm)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE imm32)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE version)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE Psapi)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE PowrProf)
  endif()
  
  if(ICEENGINE_BENCHMARKS_EXPORT_TO_CSV)
    add_test(${BENCHMARK_NAME} ${EXECUTABLE_NAME} -t ${BENCHMARK_NAME}.csv)
  else()
    add_test(${BENCHMARK_NAME} ${EXECUTABLE_NAME})
  endif()
  
  
endmacro()

create_benchmark(ScriptingEngineBenchmarks ScriptingEngineBenchmarks ScriptingEngine.cpp)
