#include <cstring>
#include <ctime>

#include "Frame.hpp"

Disk::Frame::Frame(frameid_t fid, char *data, diskmgr_t dmgr)
    : fid_(fid), bid_(0), data_(data), dirty_(false), sync_(false), dmgr_(dmgr) {}

auto Disk::Frame::frm_read(size_t offset, void *buf, size_t len) -> size_t {
  if (sync_ == false) {
    frm_sync();
  }
  auto maxlen = PAGE_SIZ - offset;
  auto rlen = len > maxlen ? maxlen : len;

  memcpy(buf, data_ + offset, rlen);

  return rlen;
}

auto Disk::Frame::frm_write(size_t offset, void *buf, size_t len) -> size_t {
  if (sync_ == false) {
    frm_sync();
  }
  dirty_ = true;

  auto maxlen = PAGE_SIZ - offset;
  auto wlen = len > maxlen ? maxlen : len;

  memcpy(data_ + offset, buf, wlen);

  return wlen;
}

auto Disk::Frame::frm_load(blockid_t bid) -> void {
  if (bid_)
    frm_flush();
  bid_ = bid;
}

auto Disk::Frame::frm_drop() -> void {
  frm_flush();
  sync_ = false;
  bid_ = 0;
  memset(data_, 0, PAGE_SIZ);
}

auto Disk::Frame::frm_sync() -> void {
  assert(bid_);

  if (sync_ == false) {
    dmgr_->blk_read(bid_, data_, PAGE_SIZ);
    sync_ = true;
  }
}

auto Disk::Frame::frm_flush() -> void {
  if (sync_ && dirty_) {
    dmgr_->blk_write(bid_, data_, PAGE_SIZ);
    dirty_ = false;
  }
}

Disk::Record::Record(frameid_t fid) : fid_(fid), count_(0), inuse_(0), free_(true) {}

//
// if A < B then evict A
//
auto Disk::Record::operator<(const Record &rhs) const -> bool {
  if (free_) {
    return true;
  }
  if (inuse_) {
    return false;
  }

  auto &&lhs = *this;

  bool lgK = lhs.count_ > KthLRU;
  bool rgK = rhs.count_ > KthLRU;

  if (lgK && rgK) {
    auto &&llastuse = lhs.tstmp_[lhs.count_ % KthLRU];
    auto &&rlastuse = rhs.tstmp_[rhs.count_ % KthLRU];

    return llastuse < rlastuse;
  } else if (!lgK && rgK) {
    return true;
  } else if (lgK && !rgK) {
    return false;
  }

  return lhs.count_ < rhs.count_;
}

//
// if A > B then evict B
//
auto Disk::Record::operator>(const Record &rhs) const -> bool {
  if (free_) {
    return false;
  }
  if (inuse_) {
    return true;
  }

  auto &&lhs = *this;

  bool lgK = lhs.count_ > KthLRU;
  bool rgK = rhs.count_ > KthLRU;

  if (lgK && rgK) {
    auto &&llastuse = lhs.tstmp_[lhs.count_ % KthLRU];
    auto &&rlastuse = rhs.tstmp_[rhs.count_ % KthLRU];

    return llastuse > rlastuse;
  } else if (!lgK && rgK) {
    return false;
  } else if (lgK && !rgK) {
    return true;
  }

  return lhs.count_ > rhs.count_;
}

auto Disk::Record::free() -> void {
  count_ = 0;
}

auto Disk::Record::release() -> void {
  inuse_ -= 1;
  if (!inuse_) {
    free_ = true;
  }
}

auto Disk::Record::hold() -> void {
  inuse_ += 1;
  free_ = false;
}

auto Disk::Record::access() -> void { tstmp_[count_++] = clock(); }
