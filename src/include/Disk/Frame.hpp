#pragma once

#include "DiskDefs.hpp"
#include "DiskManager.hpp"

namespace Disk {

struct Frame {
  frameid_t fid_;
  blockid_t bid_;
  void *data_;
  bool dirty;
  bool sync;
  diskmgr_t dmgr;

  Frame(frameid_t fid, void *data);
  auto frm_load(blockid_t bid) -> void;
  auto frm_drop(void) -> void;
  auto frm_sync(void) -> void;
  auto frm_flush(void) -> void;
  auto frm_write(addr_t addr, void *buf, size_t len) -> void;
  auto frm_read(addr_t addr, void *buf, size_t len) -> void;
};

using frame_t = Frame *;

} // namespace Disk