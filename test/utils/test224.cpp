#include "utils.hpp"
#include <string>
#include <iostream>

int main()
{
  std::string str = "password1";
  std::string str2 = calculateSHA224(str);
  std::cout << str2 << std::endl;
  return 0;
}