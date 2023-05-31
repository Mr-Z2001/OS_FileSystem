#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <openssl/sha.h>
#include <string>

std::string calculateSHA224(const std::string &str);
auto strToInt(std::string) -> int;
void getpass(char *password);

#endif