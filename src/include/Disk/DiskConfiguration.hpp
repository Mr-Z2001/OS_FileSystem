#ifndef __DISKCONFIGURATION_HPP__
#define __DISKCONFIGURATION_HPP__

#include <cstddef>

const std::size_t SECTOR_SIZE = 512;
const std::size_t SECTOR_PER_BLOCK = 16;
const std::size_t BLOCK_SIZE = SECTOR_PER_BLOCK * SECTOR_SIZE;
const std::size_t BLOCK_PER_PARTITION = 1024;
const std::size_t PARTITION_SIZE = BLOCK_PER_PARTITION * BLOCK_SIZE;

#endif