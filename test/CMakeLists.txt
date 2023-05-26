add_executable(main main.cpp)
add_executable(test224 utils/test224.cpp)
add_executable(testdata data/testdata.cpp)

target_link_libraries(test224 PUBLIC UTILS)