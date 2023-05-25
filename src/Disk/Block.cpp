#include "Block.hpp"

using std::pair;

Disk::Block::Block(blockid_t bid, const char *path) : bid_(bid), disk_(open(path, O_RDWR)) {
  lseek(disk_, bid_ * PAGE_SIZ, SEEK_SET);
}

auto Disk::Block::offset() const -> size_t { return bid_ * PAGE_SIZ; }

auto Disk::Block::blk_write(void *buf, size_t len) -> void {
  assert(len == PAGE_SIZ);

  lseek(disk_, bid_ * PAGE_SIZ, SEEK_SET);
  lockf(disk_, F_LOCK, PAGE_SIZ);
  write(disk_, buf, len);
  sync_file_range(disk_, bid_ * PAGE_SIZ, PAGE_SIZ, SYNC_FILE_RANGE_WRITE_AND_WAIT);
  lockf(disk_, F_ULOCK, PAGE_SIZ);
}

auto Disk::Block::blk_read(void *buf, size_t len) -> void {
  assert(len == PAGE_SIZ);

  lseek(disk_, bid_ * PAGE_SIZ, SEEK_SET);
  lockf(disk_, F_LOCK, PAGE_SIZ);
  read(disk_, buf, PAGE_SIZ);
  lockf(disk_, F_ULOCK, PAGE_SIZ);
}

Disk::Block::~Block() { close(disk_); }
