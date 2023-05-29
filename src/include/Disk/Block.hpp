#pragma once

#include "DiskDefs.hpp"

namespace Disk {

struct Block {
  const blockid_t bid_;
  const fd_t disk_;

  Block(blockid_t bid, const char *path);
  auto offset(void) const -> size_t;
  auto blk_write(void *buf, size_t len) -> void;
  auto blk_read(void *buf, size_t len) -> void;

  ~Block();
};

using block_t = Block *;

} // namespace Disk