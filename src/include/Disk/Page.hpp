#pragma once

#include "DiskDefs.hpp"

namespace Disk {

struct Page {
  pageid_t pgid_; // 页号
  rwxinfo plv_;   // 读写权限
  size_t offset_;
  Vec<blockid_t> bids;

  Page(pageid_t pgid, rwxinfo plv);
};

using page_t = Page *;

} // namespace Disk