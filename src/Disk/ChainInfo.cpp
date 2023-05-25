#include "ChainInfo.hpp"

Disk::BlockChainManager::BlockChainManager() {
  for (size_t i = 0; i < GROUP_SIZ; ++i) {
    bc_->status = 0;
  }
}

auto Disk::BlockChainManager::alloc(size_t nm) -> Disk::Vec<Disk::blockid_t> {
  const size_t mask = 0x1;

  Disk::BlockChain *curbc = bc_;
  Disk::Vec<Disk::blockid_t> result;

  size_t chain_id = 0;

  while (nm) {
    curbc = bc_;

    for (size_t i = 1; i < 64; ++i) {
      if (((mask << i) & curbc->status) == 0) {
        curbc = &curbc[i];
        chain_id = i;
        break;
      }
      if (i == 63) {
        assert(0 && "not enough space");
      }
    }

    for (size_t i = 0; i < 64; ++i) {
      if (((mask << i) & curbc->status) == 0) {
        result.push_back(chain_id * 64 + i);
        curbc->status = curbc->status | (mask << i);
        nm -= 1;
        if (nm == 0) {
          break;
        }
      }
    }

    for (size_t i = 1; i < 64; ++i) {
      if (bc_[i].status == (size_t)-1) {
        bc_[0].status = bc_[0].status | (mask << i);
      }
    }
  }

  return result;
}

auto Disk::BlockChainManager::release(Disk::Vec<Disk::blockid_t> &blks) -> void {
  const size_t mask = 0x1;

  for (auto &&bid : blks) {
    bc_[bid / 64].status = bc_[bid / 64].status & ~(mask << (bid % 64));
  }

  for (size_t i = 1; i < 64; ++i) {
    if (bc_[i].status != (size_t)-1) {
      bc_[0].status = bc_[0].status & ~(mask << i);
    }
  }
}