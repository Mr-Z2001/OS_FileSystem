# CMake minimum version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(FILESYSTEM)

# Set C++ standard (optional)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

include_directories(
  src/include
  src/include/File
  src/include/User
  src/include/Directory
  src/include/Disk
  src/include/Shell
  src/include/Manager
  utils
  test/include
)

configure_file(data/userdata.csv ${CMAKE_BINARY_DIR}/test/userdata.csv COPYONLY)
configure_file(data/userGroupData.csv ${CMAKE_BINARY_DIR}/test/userGroupData.csv COPYONLY)
configure_file(data/CommandToInt.csv ${CMAKE_BINARY_DIR}/test/CommandToInt.csv COPYONLY)
configure_file(data/nodeInfo.csv ${CMAKE_BINARY_DIR}/test/nodeInfo.csv COPYONLY)
configure_file(data/tree.csv ${CMAKE_BINARY_DIR}/test/tree.csv COPYONLY)
configure_file(storage/hdd.bin ${CMAKE_BINARY_DIR}/test/hdd.bin COPYONLY)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(utils)
add_subdirectory(data)
add_subdirectory(storage)

# third party
