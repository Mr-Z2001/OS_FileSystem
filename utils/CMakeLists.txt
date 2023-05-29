add_library(utils
  utils.hpp
  utils.cpp)

target_link_libraries(utils PUBLIC ssl crypto)