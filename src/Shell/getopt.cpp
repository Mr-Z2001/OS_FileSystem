#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

int main() {
  std::string line = "ls -lah /usr/local";
  std::istringstream iss(line);

  std::vector<std::string> tokens;
  std::string token;
  while (iss >> token)
    tokens.push_back(token);

  int argc = tokens.size();
  char *argv[argc];
  for (int i = 0; i < argc; ++i)
    argv[i] = const_cast<char *>(tokens[i].c_str());
  const char *optstring = "lah";
  std::string arg;

  int opt;
  while ((opt = getopt(argc, argv, optstring)) != -1) {
    switch (opt) {
    case 'l':
      std::cout << "option: l" << std::endl;
      break;
    case 'a':
      std::cout << "option: a" << std::endl;
      break;
    case 'h':
      std::cout << "option: h" << std::endl;
      break;
    default:
      arg = optarg;
      std::cout << arg << std::endl;
      std::cout << "option: ?" << std::endl;
      break;
    }
  }
  std::cout << argv[optind] << std::endl;
  return 0;
}