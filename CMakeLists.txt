# CMake minimum version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(FILESYSTEM)

# Set C++ standard (optional)
set(CMAKE_CXX_STANDARD 17)

include_directories(
  src/include/File
  src/include/User
  src/include/Directory
  test/include)

add_subdirectory(src)
add_subdirectory(test)
