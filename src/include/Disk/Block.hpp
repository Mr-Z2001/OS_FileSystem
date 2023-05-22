#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__

#include <array>

#include "Sector.hpp"

class Block
{
private:
  std::size_t size = BLOCK_SIZE; // 8KB
  std::array<Sector *, SECTOR_PER_BLOCK> sectors;

public:
  Block();
  ~Block();

  void write(int sectorIndex, std::string str);
  std::string read(int sectorIndex, int sectorCount);
  void clear();

  std::size_t getSize();
  std::size_t getSectorCount();
  Sector *getSector(int index);
};

#endif