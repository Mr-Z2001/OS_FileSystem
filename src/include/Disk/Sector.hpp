#ifndef __SECTOR_HPP__
#define __SECTOR_HPP__

#include <array>
#include <string>

#include "DiskConfiguration.hpp"

class Sector
{
private:
  static const std::size_t size = SECTOR_SIZE; // 512 bytes
  std::array<char, size> arr;

public:
  Sector();
  ~Sector();

  void write(int position, std::string str);
  std::string read(int position, std::size_t length);
  void clear();

  std::size_t getSize();
};

#endif