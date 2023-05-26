#pragma once

#include "DiskDefs.hpp"
#include "DiskManager.hpp"

namespace Disk {

struct Frame {
  frameid_t fid_;
  blockid_t bid_;
  char *data_;
  bool dirty_;
  bool sync_;
  diskmgr_t dmgr_;

  Frame(frameid_t fid, char *data, diskmgr_t dmgr);
  auto frm_load(blockid_t bid) -> void;
  auto frm_drop(void) -> void;
  auto frm_sync(void) -> void;
  auto frm_flush(void) -> void;
  auto frm_write(size_t offset, void *buf, size_t len) -> size_t;
  auto frm_read(size_t offset, void *buf, size_t len) -> size_t;
};

using frame_t = Frame *;

struct Record {
  frameid_t fid_;
  size_t count_ = 0;
  size_t inuse_ = 0;
  bool free_ = true;
  time_t tstmp_[KthLRU] = {0};

  Record(frameid_t fid);
  auto release() -> void;
  auto free() -> void;
  auto hold() -> void;
  auto access() -> void;
  auto operator<(const Record &rhs) const -> bool;
  auto operator>(const Record &rhs) const -> bool;
};

using record_t = Record *;

} // namespace Disk