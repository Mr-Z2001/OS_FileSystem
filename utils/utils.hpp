#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <openssl/sha.h>
#include <string>
#include <vector>

std::string calculateSHA224(const std::string &str);
auto strToInt(std::string) -> int;
void getpass(char *password);
std::vector<std::string> parse(std::string line);
int cstrToInt(char *c_str);
std::vector<std::string> split(std::string str, char delim);

#endif