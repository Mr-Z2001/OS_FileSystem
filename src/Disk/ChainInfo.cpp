#include "ChainInfo.hpp"

Disk::BlockChainManager::BlockChainManager() {
  for (size_t i = 0; i < GROUP_SIZ; ++i) {
    bc_[i].status = 0;
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
        auto nwstat = curbc->status | (mask << i);
        curbc->status = nwstat;
        nm -= 1;
        if (nm == 0) {
          break;
        }
      }
    }

    for (size_t i = 1; i < 64; ++i) {
      if (bc_[i].status == (size_t)-1) {
        auto nwstat = bc_[0].status | (mask << i);
        bc_[0].status = nwstat;
      }
    }
  }

  return result;
}

auto Disk::BlockChainManager::release(Disk::Vec<Disk::blockid_t> &blks) -> void {
  const size_t mask = 0x1;

  for (auto &&bid : blks) {
    auto nwstat = bc_[bid / 64].status & ~(mask << (bid % 64));
    bc_[bid / 64].status = nwstat;
  }

  for (size_t i = 1; i < 64; ++i) {
    if (bc_[i].status != (size_t)-1) {
      auto nwstat = bc_[0].status & ~(mask << i);
      bc_[0].status = nwstat;
    }
  }
}

auto Disk::BlockChainManager::mark(blockid_t bid) -> void {
  const size_t mask = 0x1;
  auto nwstat = bc_[bid / 64].status | (mask << (bid % 64));
  bc_[bid / 64].status = nwstat;
}