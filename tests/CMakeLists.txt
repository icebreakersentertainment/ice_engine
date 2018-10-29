cmake_minimum_required(VERSION 3.1.0)

project(ice_engine_tests)

find_package(Boost 1.68.0 REQUIRED COMPONENTS unit_test_framework)

find_library(BOOST_UNIT_TEST_FRAMEWORK_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_unit_test_framework${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_unit_test_framework${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_unit_test_framework${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)

if(APPLE)
  find_library(BOOST_TIMER_LIBRARY NAMES
    ${BOOST_FILENAME_PREFIX}boost_timer${BOOST_FILENAME_POSTFIX}.dylib
    PATHS ${BOOST_LIBRARY_DIRS}
  )
endif()

if(WIN32)
  configure_file(../deps/freeimage/lib/FreeImage.dll ./ COPYONLY)
  configure_file(../deps/glew/lib/glew32.dll ./ COPYONLY)
  configure_file(../deps/assimp/lib/assimp-vc140-mt.dll ./ COPYONLY)
  file(COPY ../tests/fixtures DESTINATION ./)
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
  configure_file(${BOOST_UNIT_TEST_FRAMEWORK_LIBRARY} ./ COPYONLY)
  configure_file(${ASSIMP_LIBRARY} ./ COPYONLY)
  configure_file(${ENTITYX_LIBRARY} ./ COPYONLY)
  file(COPY ../tests/fixtures DESTINATION ./)
endif()

if(APPLE)
  configure_file(${BOOST_SERIALIZATION_LIBRARY} ./ COPYONLY)
  configure_file(${BOOST_TIMER_LIBRARY} ./ COPYONLY)
endif()

macro(create_test TEST_NAME EXECUTABLE_NAME SOURCE)
  add_executable(${EXECUTABLE_NAME} "src/${SOURCE}")
  
  target_include_directories(${EXECUTABLE_NAME} PRIVATE ${ICEENGINE_INCLUDE_DIRS})
  target_include_directories(${EXECUTABLE_NAME} PRIVATE ${ANGELSCRIPT_INCLUDE_DIRS})
  target_include_directories(${EXECUTABLE_NAME} PRIVATE ${GLM_INCLUDE_DIRS})
  target_include_directories(${EXECUTABLE_NAME} PRIVATE ${BOOST_INCLUDE_DIRS})
  
  add_dependencies(${EXECUTABLE_NAME} ice_engine)

  if(MSVC)
    target_link_libraries(${EXECUTABLE_NAME} ../${CMAKE_BUILD_TYPE}/ice_engine)
  else()
    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE -DBOOST_TEST_DYN_LINK)
    target_link_libraries(${EXECUTABLE_NAME} ice_engine)
  endif()
  
	target_link_libraries(${EXECUTABLE_NAME} ${FREEIMAGE_LIBRARIES} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_SYSTEM_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_CHRONO_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_ATOMIC_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_THREAD_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_PROGRAM_OPTIONS_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_FILESYSTEM_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_LOG_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_LOG_SETUP_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_DATE_TIME_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_REGEX_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_SERIALIZATION_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${ASSIMP_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${SDL2_LIBRARIES} )
	target_link_libraries(${EXECUTABLE_NAME} ${ANGELSCRIPT_LIBRARY} )
	target_link_libraries(${EXECUTABLE_NAME} ${ENTITYX_LIBRARY} )
  
  target_link_libraries(${EXECUTABLE_NAME} Threads::Threads)
  target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
  if(UNIX AND NOT APPLE)
    target_link_libraries(${EXECUTABLE_NAME} ${X11_LIBRARIES})
    target_link_libraries(${EXECUTABLE_NAME} ${X11_Xext_LIB})
    target_link_libraries(${EXECUTABLE_NAME} ${X11_Xxf86vm_LIB})
  endif()
  target_link_libraries(${EXECUTABLE_NAME} ${CMAKE_DL_LIBS})
  
  if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME} winmm)
    target_link_libraries(${EXECUTABLE_NAME} imm32)
    target_link_libraries(${EXECUTABLE_NAME} version)
    target_link_libraries(${EXECUTABLE_NAME} Psapi)
  endif()
  
  add_test(${TEST_NAME} ${EXECUTABLE_NAME})
endmacro()

create_test(FileSystemTests FileSystemTests fs/FileSystem.cpp)
create_test(ScriptingEngineTests ScriptingEngineTests scripting/ScriptingEngine.cpp)
create_test(ParameterTests ParameterTests scripting/Parameter.cpp)
