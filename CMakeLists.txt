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
  src/include/Disk
  utils
  test/include
)

configure_file(data/userdata.csv ${CMAKE_BINARY_DIR}/src/userdata.csv COPYONLY)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(utils)
add_subdirectory(data)
add_subdirectory(storage)

# third party
add_subdirectory(third_party/cryptopp-cmake)
