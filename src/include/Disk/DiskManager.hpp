#pragma once

#include <unordered_map>

#include "Block.hpp"
#include "ChainInfo.hpp"
#include "DiskDefs.hpp"
#include "Frame.hpp"
#include "ChainInfo.hpp"
#include "Page.hpp"

namespace Disk {

class DiskManager {
  fd_t disk_;
  
  BlockChainManager *bcm;

  block_t blks[BLOK_NM];

  frame_t frms[FRME_NM];
  record_t rcds[FRME_NM];

  std::unordered_map<pageid_t, page_t> pagemap;
  std::unordered_map<blockid_t, frameid_t> hashmap;

  auto frm_alloc(blockid_t bid) -> frameid_t;
  auto frm_release(frameid_t fid) -> void;
  auto frm_write(frameid_t fid, size_t offset, void *buf, size_t len) -> size_t;
  auto frm_read(frameid_t fid, size_t offset, void *buf, size_t len) -> size_t;

public:
  DiskManager(const char *path);
  ~DiskManager();

  auto blk_alloc(size_t nm) -> Vec<blockid_t>;
  auto blk_release(Vec<blockid_t> &bids) -> void;
  auto blk_write(blockid_t bid, void *buf, size_t len) -> void;
  auto blk_read(blockid_t bid, void *buf, size_t len) -> void;

  auto pg_alloc(Vec<blockid_t> &nm, rwxinfo plv) -> pageid_t;
  auto pg_release(pageid_t pgid) -> Vec<blockid_t>;
  auto pg_seek(pageid_t pgid, size_t offset, mode_t mod) -> void;
  auto pg_flush(pageid_t pgid) -> void;
  auto pg_write(pageid_t pgid, void *buf, size_t len) -> void;
  auto pg_read(pageid_t pgid, void *buf, size_t len) -> void;
};

using diskmgr_t = DiskManager *;

} // namespace Disk