# Includes.
set(FS_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(FS_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)

# set(FS_THIRD_PARTY_INCLUDE_DIR
# ${PROJECT_SOURCE_DIR}/third_party
# )
include_directories(${FS_SRC_INCLUDE_DIR} ${FS_TEST_INCLUDE_DIR} ${FS_THIRD_PARTY_INCLUDE_DIR})
include_directories(BEFORE src) # This is needed for gtest.
