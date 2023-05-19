cmake_minimum_required(VERSION 3.10)

project(FS
  VERSION 1.0
  DESCRIPTION "File System"
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(src/include test/include)

add_subdirectory(src)
add_subdirectory(test)

link_directories(src)

add_executable(defaultTest test/defaultTest.cpp)

target_link_libraries(defaultTest
  PRIVATE
  dc)
