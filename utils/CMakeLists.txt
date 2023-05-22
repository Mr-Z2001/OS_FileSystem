add_library(UTILS
  utils.hpp
  utils.cpp)

target_link_libraries(UTILS PUBLIC cryptopp)