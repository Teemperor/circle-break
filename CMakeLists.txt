cmake_minimum_required(VERSION 3.2)
project(circle-break)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(GTest)

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
    src/IncludePaths.h src/MiscUtils.h
    src/DependencyPath.cpp
    src/DependencyPath.h
    src/FileSystemCache.cpp
    src/FileSystemCache.h
    src/HtmlReport.cpp
    src/HtmlReport.h
)

add_library(circle-break-logic ${SOURCE_FILES})
target_link_libraries(circle-break-logic
                      ${Boost_FILESYSTEM_LIBRARY}
                      ${Boost_SYSTEM_LIBRARY}
)

add_executable(circle-break main.cpp)
target_link_libraries(circle-break circle-break-logic)

if (${GTEST_FOUND})

  add_executable(circle-break-tests
                 tests/test.cpp
                 tests/StringCacheTest.h
                 tests/HeaderIncludeTest.h
                 tests/HeaderTest.h
                 tests/ModuleTest.h
                 tests/ProjectTest.h
  )

  include_directories(${GTEST_INCLUDE_DIRS})

  target_link_libraries(circle-break-tests circle-break-logic ${GTEST_BOTH_LIBRARIES})
endif(${GTEST_FOUND})