#pragma once

#include "Block.hpp"
#include "DiskDefs.hpp"
#include "Frame.hpp"
#include "ChainInfo.hpp"

namespace Disk {

class DiskManager {
  fd_t disk_;
  BlockChainManager *bcm;

  block_t blks[BLOK_NM];
  frame_t frms[FRME_NM];

public:
  DiskManager(const char *path);
  auto blk_alloc(size_t nm) -> Vec<blockid_t>;
  auto blk_release(Vec<blockid_t> &bids) -> void;

  auto blk_write(blockid_t bid, void *buf, size_t len) -> void;
  auto blk_read(blockid_t bid, void *buf, size_t len) -> void;
  ~DiskManager();
};

using diskmgr_t = DiskManager *;

} // namespace Disk