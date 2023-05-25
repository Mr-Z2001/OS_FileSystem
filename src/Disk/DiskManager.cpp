#include "DiskManager.hpp"

Disk::DiskManager::DiskManager(const char *path) : disk_(open(path, O_RDONLY)) {
  for (size_t i = 0; i < BLOK_NM; ++i) {
    blks[i] = new Block(i, path);
  }
  for (size_t i = 0; i < FRME_NM; ++i) {
    char *data = new char[PAGE_SIZ];
    frms[i] = new Frame(i, (void *)data);
  }
}

auto Disk::DiskManager::blk_alloc(size_t nm) -> Disk::Vec<Disk::blockid_t> {
  return bcm->alloc(nm);
}

auto Disk::DiskManager::blk_release(Disk::Vec<Disk::blockid_t> &bids) -> void {
  bcm->release(bids);
}

auto Disk::DiskManager::blk_write(blockid_t bid, void *buf, size_t len) -> void {
  assert(bid && bid < BLOK_NM);
  blks[bid]->blk_write(buf, len);
}

auto Disk::DiskManager::blk_read(blockid_t bid, void *buf, size_t len) -> void {
  assert(bid && bid < BLOK_NM);
  blks[bid]->blk_read(buf, len);
}

Disk::DiskManager::~DiskManager() {
  delete[] blks;
  delete[] frms;
  close(disk_);
}