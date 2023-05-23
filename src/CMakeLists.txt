set(FILE_SOURCES
  File/FileFullName.cpp
  File/FileContext.cpp
  File/FileType.cpp
  File/FileAttribute.cpp
  File/FilePermission.cpp
  File/File.cpp
  File/FileManager.cpp
)

set(FILE_HEADERS
  include/File/FileFullName.hpp
  include/File/FileContext.hpp
  include/File/FileType.hpp
  include/File/FileAttribute.hpp
  include/File/FilePermission.hpp
  include/File/File.hpp
  include/File/FileManager.hpp
)

set(USER_SOURCES
  User/User.cpp
  User/UserManager.cpp
  User/UserGroup.cpp
)

set(USER_HEADERS
  include/User/User.hpp
  include/User/UserManager.hpp
  include/User/UserGroup.hpp
)

set(DIRECTORY_SOURCES
  Directory/Directory.cpp
)

set(DIRECTORY_HEADERS
  include/Directory/Directory.hpp
)

set(DISK_SOURCES
  Disk/Disk.cpp
  Disk/DiskManager.cpp
  Disk/Sector.cpp
  Disk/Partition.cpp
  Disk/Block.cpp
)

set(DISK_HEADERS
  include/Disk/Disk.hpp
  include/Disk/DiskManager.hpp
  include/Disk/Sector.hpp
  include/Disk/Partition.hpp
  include/Disk/Block.hpp
  include/Disk/DiskConfiguration.hpp
)

set(SHELL_SOURCES
  Shell/Shell.cpp)

set(SHELL_HEADERS
  include/Shell/Shell.hpp)

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

add_library(disk
  ${DISK_SOURCES}
  ${DISK_HEADERS}
)

add_library(shell
  ${SHELL_SOURCES}
  ${SHELL_HEADERS}
)

target_link_libraries(user PUBLIC cryptopp)