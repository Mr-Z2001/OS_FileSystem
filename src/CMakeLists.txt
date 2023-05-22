set(
  FILE_SOURCES
  File/FileFullName.cpp
  File/FileContext.cpp
  File/FileType.cpp
  File/FileAttribute.cpp
  File/FilePermission.cpp
  File/File.cpp
  File/FileManager.cpp
)

set(
  FILE_HEADERS
  include/File/FileFullName.hpp
  include/File/FileContext.hpp
  include/File/FileType.hpp
  include/File/FileAttribute.hpp
  include/File/FilePermission.hpp
  include/File/File.hpp
  include/File/FileManager.hpp
)

set(
  USER_SOURCES
  User/User.cpp
  User/UserManager.cpp
)

set(USER_HEADERS
  include/User/User.hpp
  include/User/UserManager.hpp
)

set(
  DIRECTORY_SOURCES
  Directory/Directory.cpp
)

set(
  DIRECTORY_HEADERS
  include/Directory/Directory.hpp
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

target_link_libraries(user PUBLIC cryptopp)