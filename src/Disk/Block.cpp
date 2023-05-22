#include "Block.hpp"
#include <vector>

Block::Block() {}

Block::~Block() {}

void Block::write(int sectorIndex, std::string str)
{
  std::vector<std::string> strings;
  for (int i = 0; i < str.length(); i += SECTOR_SIZE)

    strings.push_back(str.substr(i, SECTOR_SIZE));

  for (int i = 0; i < strings.size(); i++)
    sectors[sectorIndex + i]->write(0, strings[i]);
}

std::string Block::read(int sectorIndex, int sectorCount)
{
  std::string str;
  for (int i = 0; i < sectorCount; i++)
    str += sectors[sectorIndex + i]->read(0, SECTOR_SIZE);
  return str;
}

void Block::clear()
{
  for (int i = 0; i < SECTOR_PER_BLOCK; i++)
    sectors[i]->clear();
}

std::size_t Block::getSize()
{
  return size;
}

std::size_t Block::getSectorCount()
{
  return SECTOR_PER_BLOCK;
}

Sector *Block::getSector(int index)
{
  return sectors[index];
}
