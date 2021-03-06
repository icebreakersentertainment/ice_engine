set(BOOST_VERSION "1.68.0")
set(BOOST_FILENAME_PREFIX "lib")
set(BOOST_FILENAME_POSTFIX "")
if(MSVC)
  set(BOOST_FILENAME_PREFIX "lib")
  set(BOOST_FILENAME_POSTFIX "-vc141-mt-x64-1_68")
endif()

set(BOOST_ROOT "${CMAKE_CURRENT_LIST_DIR}/../deps/boost")
set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
set(BOOST_LIBRARY_DIRS  "${BOOST_ROOT}/lib")

if(MSVC)
  set(Boost_USE_STATIC_LIBS ON)
else()
  set(Boost_USE_STATIC_LIBS OFF)
endif()

set(Boost_USE_MULTITHREADED ON)

find_package(Boost 1.68.0 REQUIRED COMPONENTS filesystem system atomic chrono thread program_options log log_setup date_time regex serialization wave)

find_library(BOOST_SYSTEM_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_system${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_system${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_system${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_SYSTEM_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_system${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_system${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_system${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_CHRONO_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_chrono${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_chrono${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_chrono${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_CHRONO_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_chrono${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_chrono${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_chrono${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_ATOMIC_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_atomic${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_atomic${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_atomic${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_ATOMIC_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_atomic${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_atomic${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_atomic${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_PROGRAM_OPTIONS_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_program_options${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_program_options${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_program_options${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_PROGRAM_OPTIONS_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_program_options${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_program_options${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_program_options${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_FILESYSTEM_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_filesystem${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_filesystem${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_filesystem${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_FILESYSTEM_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_filesystem${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_filesystem${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_filesystem${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_THREAD_LIBRARY_FILENAME NAMES
  ${BOOST_FILENAME_PREFIX}boost_thread${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_thread${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_thread${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_THREAD_LIBRARY_FILENAME NAMES
  ${BOOST_FILENAME_PREFIX}boost_thread${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_thread${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_thread${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_LOG_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_log${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_log${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_log${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_LOG_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_log${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_log${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_log${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_LOG_SETUP_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_log_setup${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_log_setup${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_log_setup${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_LOG_SETUP_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_log_setup${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_log_setup${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_log_setup${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_DATE_TIME_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_date_time${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_date_time${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_date_time${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_DATE_TIME_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_date_time${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_date_time${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_date_time${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_REGEX_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_regex${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_regex${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_regex${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_REGEX_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_regex${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_regex${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_regex${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_SERIALIZATION_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_serialization${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_serialization${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_serialization${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
  NO_DEFAULT_PATH
)
find_library(BOOST_SERIALIZATION_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_serialization${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_serialization${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_serialization${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)
find_library(BOOST_WAVE_LIBRARY NAMES
  ${BOOST_FILENAME_PREFIX}boost_wave${BOOST_FILENAME_POSTFIX}.so.${BOOST_VERSION}
  ${BOOST_FILENAME_PREFIX}boost_wave${BOOST_FILENAME_POSTFIX}.dylib
  ${BOOST_FILENAME_PREFIX}boost_wave${BOOST_FILENAME_POSTFIX}
  PATHS ${BOOST_LIBRARY_DIRS}
)