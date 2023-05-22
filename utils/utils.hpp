#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <string>
#include "cryptopp/sha.h"
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>

std::string calculateSHA224(const std::string &message);

#endif