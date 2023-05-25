#ifndef __SHELLFUNCTIONS_HPP__
#define __SHELLFUNCTIONS_HPP__

#include <vector>
#include <string>
#include <sstream>
#include <map>

std::vector<std::string> parse(std::string line)
{
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream iss(line);
  while (iss >> token)
    tokens.push_back(token);
  return tokens;
}

int cstrToInt(char *c_str)
{
  std::string str(c_str);
  return std::stoi(str);
}

#endif