#include "ShellFunctions.hpp"

std::vector<std::string> parse(std::string line) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream iss(line);
  while (iss >> token)
    tokens.push_back(token);
  tokens.push_back("");
  return tokens;
}

int cstrToInt(char *c_str) {
  if (c_str == NULL)
    return -1;
  std::string str(c_str);
  return std::stoi(str);
}
