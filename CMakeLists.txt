cmake_minimum_required(VERSION 3.7)
project(cycledetect)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

set(SOURCE_FILES main.cpp
    src/Header.cpp
    src/Header.h
    src/HeaderInclude.cpp
    src/HeaderInclude.h
    src/Module.cpp
    src/Module.h
    src/StringCache.cpp
    src/StringCache.h
    src/Project.cpp
    src/Project.h
    src/IncludePaths.cpp
    src/IncludePaths.h)

add_library(cycledetect-logic ${SOURCE_FILES})
target_link_libraries(cycledetect-logic boost_filesystem boost_system)

add_executable(cycledetect main.cpp)
target_link_libraries(cycledetect cycledetect-logic)

add_executable(cycledetect-tests
               tests/test.cpp
               tests/StringCacheTest.h
               tests/HeaderIncludeTest.h
               tests/HeaderTest.h
               tests/ModuleTest.h
               )
target_link_libraries(cycledetect-tests cycledetect-logic gtest)