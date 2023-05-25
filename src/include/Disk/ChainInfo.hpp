#pragma once

#include "DiskDefs.hpp"

extern int __builtin_popcountll(long long int);

namespace Disk {

static_assert(sizeof(size_t) == 8);

/**
 * @brief index
 * 
 *  xxxx'xxxx'...xxxx'xxxx -> 64 groups status
 *  if groups in block i full
 *      the groups[i] = true;
 *  group is full => status = -1
 * 
 * depth == 2
 */

[[ maybe_unused ]] constexpr size_t CHAIN_DEPTH  = 2;

constexpr size_t GROUP_SIZ = 1 << 6;

struct BlockChain {
    size_t status;
};

class BlockChainManager {
    BlockChain bc_[GROUP_SIZ];
public:
    BlockChainManager();
    auto alloc(size_t nm) -> Vec<blockid_t>;
    auto release(Vec<blockid_t> &blks) -> void;
};

}