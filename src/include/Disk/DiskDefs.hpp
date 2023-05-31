#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

#include <utility>
#include <vector>

#include <fcntl.h>
#include <unistd.h>

namespace Disk {

constexpr size_t KthLRU = 7;

constexpr size_t Byte = sizeof(char);
constexpr size_t KiB = 1024 * Byte;
constexpr size_t MiB = 1024 * KiB;
constexpr size_t GiB = 1024 * MiB;

/**
 * @brief 地址分配
 *      total size: 4 GiB
 *      page size:  4 KiB
 *      | 20 bit block id | 12 bit |
 *
 */

constexpr size_t BLOK_NM = 1 << 9;
constexpr size_t FRME_NM = 1 << 4;
constexpr size_t PAGE_SIZ = 4 * KiB;

using blockid_t = int;
using frameid_t = uint64_t;
using pageid_t = uint64_t;
using addr_t = intptr_t;

using rwxinfo = uint64_t;
using fd_t = int;

template <typename T> using Vec = std::vector<T>;

template <typename T> using resp_t = std::pair<bool, T>;

enum privilege {
  WEN = 0b001,
  REN = 0b010,
  XEN = 0b100,
};

class DiskManager;
using diskmgr_t = DiskManager *;

struct Frame;
using frame_t = Frame *;

struct Record;
using record_t = Record *;

struct Page;
using page_t = Page *;

struct Block;
using block_t = Block *;

} // namespace Disk