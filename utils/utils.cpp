#include "utils.hpp"
#include <iomanip>
#include <sstream>

std::string calculateSHA224(const std::string &str) {
  unsigned char digest[SHA224_DIGEST_LENGTH];

  SHA224(reinterpret_cast<const unsigned char *>(str.c_str()), str.length(), digest);

  std::stringstream ss;
  for (int i = 0; i < SHA224_DIGEST_LENGTH; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
  }

  return ss.str();
}