#include "Partition.hpp"

//! variables connected to `size` have errors.

Partition::Partition(std::size_t size)
{
  this->size = size;
  // for (int i = 0; i < size; i++)
  //   data.push_back(new Block());
}

Partition::~Partition()
{
  // for (int i = 0; i < size; i++)
  //   delete blocks[i];
}

void Partition::write(int blockIndex, std::string str)
{
  std::vector<std::string> strings;
  for (int i = 0; i < str.length(); i += BLOCK_SIZE)
    strings.push_back(str.substr(i, BLOCK_SIZE));
  for (int i = 0; i < strings.size(); ++i)
    data[blockIndex + i]->write(0, strings[i]);
}

std::string Partition::read(int blockIndex, int blockCount)
{
  std::string str;
  for (int i = 0; i < blockCount; i++)
    str += data[blockIndex + i]->read(0, BLOCK_SIZE);
  return str;
}

void Partition::clear()
{
  superBlock->clear();
  for (int i = 0; i < index.size(); ++i)
    index[i]->clear();
  for (int i = 0; i < data.size(); ++i)
    data[i]->clear();
}

std::size_t Partition::getSize()
{
  return size;
}

std::size_t Partition::getBlockCount()
{
  return 1 + index.size() + data.size();
}

std::size_t Partition::getSectorCount()
{
  return size * SECTOR_PER_BLOCK;
}

Block *Partition::getBlock(int index)
{
  return data[index];
}
