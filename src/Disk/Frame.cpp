#include <cstring>

#include "Frame.hpp"

Disk::Frame::Frame(frameid_t fid, void *data): fid_(fid), data_(data) {}

auto Disk::Frame::frm_read(addr_t addr, void *buf, size_t len) -> void {
    if (sync == false) {
        frm_sync();
    }

    const auto curblk_begin = bid_ * PAGE_SIZ;
    const auto nxtblk_begin = (bid_ + 1) * PAGE_SIZ;

    if (addr >=  nxtblk_begin || (addr + len) <= curblk_begin)
        return;

    /**
     * @brief write logic
     *      |   <mem>   |
     *  ^             ^
     *  addr          addr+len
     *      ^         ^
     *      begin     end
     */

    auto begin = addr < curblk_begin ? curblk_begin : addr;
    auto end   = addr + len > nxtblk_begin ? nxtblk_begin : addr + len;
    auto wlen  = end - begin;
    
    auto curoff = begin - curblk_begin;
    auto bufoff = begin - addr;

    memcpy(buf + bufoff, data_ + curoff, wlen);
}

auto Disk::Frame::frm_write(addr_t addr, void *buf, size_t len) -> void {
    if (sync == false) {
        frm_sync();
    }

    const auto curblk_begin = bid_ * PAGE_SIZ;
    const auto nxtblk_begin = (bid_ + 1) * PAGE_SIZ;

    if (addr >=  nxtblk_begin || (addr + len) <= curblk_begin)
        return;

    /**
     * @brief write logic
     *      |   <mem>   |
     *  ^             ^
     *  addr          addr+len
     *      ^         ^
     *      begin     end
     */

    auto begin = addr < curblk_begin ? curblk_begin : addr;
    auto end   = addr + len > nxtblk_begin ? nxtblk_begin : addr + len;
    auto wlen  = end - begin;
    
    auto curoff = begin - curblk_begin;
    auto bufoff = begin - addr;

    memcpy(data_ + curoff, buf + bufoff, wlen);
}

auto Disk::Frame::frm_load(blockid_t bid) -> void {
    if (bid_)
        frm_flush();
    bid_ = bid;
}

auto Disk::Frame::frm_drop() -> void {
    frm_flush();
    sync = false;
}

auto Disk::Frame::frm_sync() -> void {
    assert(bid_);

    if (sync == false) {
        dmgr->blk_read(bid_, data_, PAGE_SIZ);
        sync = true;
    }
}

auto Disk::Frame::frm_flush() -> void {
    if (sync && dirty) {
        dmgr->blk_write(bid_, data_, PAGE_SIZ);
        dirty = false;
    }
}
