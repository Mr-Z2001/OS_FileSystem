include_directories(include)

aux_source_directory(File FILE_SOURCES)
aux_source_directory(User USER_SOURCES)
aux_source_directory(Directory DIRECTORY_SOURCES)
aux_source_directory(Disk DISK_SOURCES)
aux_source_directory(Shell SHELL_SOURCES)

add_library(user ${USER_SOURCES})
add_library(fs ${FILE_SOURCES})
add_library(directory ${DIRECTORY_SOURCES})
add_library(disk ${DISK_SOURCES})
add_library(shell ${SHELL_SOURCES})

target_link_libraries(user PUBLIC cryptopp)