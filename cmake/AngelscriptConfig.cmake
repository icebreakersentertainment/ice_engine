set(ANGELSCRIPT_INCLUDE_DIRS "${CMAKE_CURRENT_LIST_DIR}/../deps/angelscript/include;${CMAKE_CURRENT_LIST_DIR}/../deps/angelscript/include/angelscript")

find_library(ANGELSCRIPT_LIBRARY NAMES angelscript PATHS ${CMAKE_CURRENT_LIST_DIR}/../deps/angelscript/lib)