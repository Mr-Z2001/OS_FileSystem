#ifndef __UTILS_HPP__
#define __UTILS_HPP__

// #include "cryptopp/sha.h"
// #include <cryptopp/cryptlib.h>
// #include <cryptopp/hex.h>
// #include <cryptopp/sha.h>
#include <string>

#include <openssl/sha.h>

std::string calculateSHA224(const std::string &str);
auto strToInt(std::string) -> int;

#endif