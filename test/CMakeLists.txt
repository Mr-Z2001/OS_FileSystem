set(
  TEST_SOURCES
  test.cpp
)

add_executable(test ${TEST_SOURCES})
target_link_libraries(test PUBLIC fs)