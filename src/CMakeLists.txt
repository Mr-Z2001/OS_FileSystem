set(
  FILE_SOURCES
  File/FileFullName.cpp
  File/FileContext.cpp
  File/FileType.cpp
  File/FileAttribute.cpp
  File/FilePermission.cpp
)

set(
  FILE_HEADERS
  include/File/FileFullName.hpp
  include/File/FileContext.hpp
  include/File/FileType.hpp
  include/File/FileAttribute.hpp
  include/File/FilePermission.hpp
)

set(
  USER_SOURCES
  User/User.cpp
)

set(USER_HEADERS
  include/User/User.hpp
)

set(
  DIRECTORY_SOURCES
  Directory/DirectorySystem.cpp
)

set(
  DIRECTORY_HEADERS
  include/Directory/DirectorySystem.hpp
)

add_library(user
  ${USER_SOURCES}
  ${USER_HEADERS})

add_library(fs
  ${FILE_SOURCES}
  ${FILE_HEADERS}
)
add_library(directory
  ${DIRECTORY_SOURCES}
  ${DIRECTORY_HEADERS}
)