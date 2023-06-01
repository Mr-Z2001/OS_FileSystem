#include "utils.hpp"
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

std::string calculateSHA224(const std::string &str) {
  unsigned char digest[SHA224_DIGEST_LENGTH];

  SHA224(reinterpret_cast<const unsigned char *>(str.c_str()), str.length(), digest);

  std::stringstream ss;
  for (int i = 0; i < SHA224_DIGEST_LENGTH; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
  }

  return ss.str();
}

auto strToInt(std::string str) -> int {
  int x = 0;
  if (str.back() == '\r') {
    str.pop_back();
  }
  for (auto c : str)
    x = (x << 3) + (x << 1) + c - '0';
  return x;
}

int my_getch() {
  int ch;
  struct termios old_settings, new_settings;
  tcgetattr(STDIN_FILENO, &old_settings);
  new_settings = old_settings;
  new_settings.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
  return ch;
}

void getpass(char *password) {
  int i = 0;
  int ch;

  while ((ch = my_getch()) != '\n') {
    if (ch == 127 || ch == 8) {
      if (i != 0) {
        i--;
        printf("\b \b");
      }
    } else {
      password[i++] = ch;
      printf("*");
    }
  }
  password[i] = '\0';
}

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

std::vector<std::string> split(std::string str, char delim) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream iss(str);
  while (std::getline(iss, token, delim))
    tokens.push_back(token);
  return tokens;
}

std::vector<std::string> split(char *str, char delim) { return split(std::string(str), delim); }