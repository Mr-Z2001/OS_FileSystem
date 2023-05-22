#ifndef __PARTITION_HPP__
#define __PARTITION_HPP__

#include "Block.hpp"

#include <vector>

class Partition
{
private:
  std::size_t size;
  Block *superBlock;
  std::vector<Block *> index;
  std::vector<Block *> data;

public:
  // default 1024*block_size
  Partition(std::size_t size = PARTITION_SIZE);
  ~Partition();

  void write(int blockIndex, std::string str);
  std::string read(int blockIndex, int sectorCount);
  void clear();

  std::size_t getSize();
  std::size_t getBlockCount();
  std::size_t getSectorCount();

  Block *getBlock(int index);
};

#endif