#include "utils.hpp"

std::string calculateSHA224(const std::string &message)
{
  CryptoPP::SHA224 hash;
  std::string digest;

  CryptoPP::StringSource(message, true,
                         new CryptoPP::HashFilter(
                             hash, new CryptoPP::HexEncoder(
                                       new CryptoPP::StringSink(digest), false)));

  return digest;
}