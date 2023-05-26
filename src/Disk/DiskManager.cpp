#include <queue>

#include "DiskManager.hpp"


Disk::DiskManager::DiskManager(const char *path) : disk_(open(path, O_RDONLY)) {
  bcm = new BlockChainManager();

  for (size_t i = 0; i < BLOK_NM; ++i) {
    blks[i] = new Block(i, path);
  }
  for (size_t i = 0; i < FRME_NM; ++i) {
    char *data = new char[PAGE_SIZ];
    frms[i] = new Frame(i, data, this);
  }
  for (size_t i = 0; i < FRME_NM; ++i) {
    rcds[i] = new Record(i);
  }
}

Disk::DiskManager::~DiskManager() {
  delete bcm;

  for (size_t i = 0; i < BLOK_NM; ++i) {
    delete blks[i];
    blks[i] = nullptr;
  }
  for (size_t i = 0; i < FRME_NM; ++i) {
    delete frms[i]->data_;
    frms[i]->data_ = nullptr;
    delete frms[i];
    frms[i] = nullptr;
  }
  for (size_t i = 0; i < FRME_NM; ++i) {
    delete rcds[i];
    rcds[i] = nullptr;
  }

  for (auto &&p: pagemap) {
    delete p.second;
    p.second = nullptr;
  }

  close(disk_);
}

auto Disk::DiskManager::blk_alloc(size_t nm) -> Disk::Vec<Disk::blockid_t> { return bcm->alloc(nm); }

auto Disk::DiskManager::blk_release(Disk::Vec<Disk::blockid_t> &bids) -> void { bcm->release(bids); }

auto Disk::DiskManager::blk_write(blockid_t bid, void *buf, size_t len) -> void {
  assert(bid && bid < BLOK_NM);
  blks[bid]->blk_write(buf, len);
}

auto Disk::DiskManager::blk_read(blockid_t bid, void *buf, size_t len) -> void {
  assert(bid && bid < BLOK_NM);
  blks[bid]->blk_read(buf, len);
}

auto Disk::DiskManager::pg_alloc(Vec<blockid_t> &nm, rwxinfo plv) -> pageid_t {
  static size_t unique_id_alloc = 0;
  page_t pg = new Page(unique_id_alloc ++, plv);
  pg->bids.insert(pg->bids.end(), nm.begin(), nm.end());
  pagemap[pg->pgid_] = pg;
  return pg->pgid_;
}

auto Disk::DiskManager::pg_release(pageid_t pgid) -> Vec<blockid_t> {
  page_t pg = pagemap.at(pgid);

  Vec<blockid_t> result = std::move(pg->bids);
  delete pg;
  pagemap[pgid] = nullptr;
  pagemap.erase(pgid);
  return result;
}

auto Disk::DiskManager::pg_seek(pageid_t pgid, size_t offset, mode_t mod) -> void {
  page_t pg = pagemap.at(pgid);

  switch (mod) {
  case SEEK_CUR:
    pg->offset_ += offset;
    break;
  case SEEK_END:
    pg->offset_ = pg->bids.size() * PAGE_SIZ + offset;
    break;
  case SEEK_SET:
    pg->offset_ = offset;
    break;
  default:
    assert(0);
  }
}

auto Disk::DiskManager::pg_flush(pageid_t pgid) -> void {
  page_t pg = pagemap.at(pgid);
  
  for (auto &&bid: pg->bids) {
    auto &&p = hashmap.find(bid);
    if (p != hashmap.end()) {
      frms[p->second]->frm_flush();
    }
  }
}

auto Disk::DiskManager::pg_write(pageid_t pgid, void *buf, size_t len) -> void {
  page_t pg = pagemap.at(pgid);
  auto vblen = pg->bids.size();
  auto bidx = pg->offset_ / PAGE_SIZ;
  auto off = pg->offset_ % PAGE_SIZ;
  auto bufoff = 0;

  for (size_t i = bidx; i < vblen; ++i) {
    auto blk = pg->bids[i];
    if (hashmap.find(blk) == hashmap.end()) {
      auto nwfrm = frm_alloc(blk);
      hashmap[blk] = nwfrm;
    }
    frameid_t frm = hashmap[blk];
    auto written = frm_write(frm, off, &((char *)buf)[bufoff], len);
    pg->offset_ += written;
    bufoff += written;
    len -= written;
    off = 0;

    if (len == 0) {
      return;
    }
  }

  auto extralen = len + pg->offset_ - PAGE_SIZ * vblen;

  auto bids = blk_alloc(extralen / PAGE_SIZ + !!(extralen % PAGE_SIZ));
  pg->bids.insert(bids.end(), bids.begin(), bids.end());

  for (size_t i = vblen; i < pg->bids.size(); ++i) {
    auto blk = pg->bids[i];
    if (hashmap.find(blk) == hashmap.end()) {
      auto nwfrm = frm_alloc(blk);
      hashmap[blk] = nwfrm;
    }
    frameid_t frm = hashmap[blk];
    auto written = frm_write(frm, off, &((char *)buf)[bufoff], len);
    pg->offset_ += written;
    bufoff += written;
    len -= written;
    off = 0;

    if (len == 0) {
      return;
    }
  }
}

auto Disk::DiskManager::pg_read(pageid_t pgid, void *buf, size_t len) -> void {
  page_t pg = pagemap.at(pgid);
  auto vblen = pg->bids.size();
  auto bidx = pg->offset_ / PAGE_SIZ;
  auto off = pg->offset_ % PAGE_SIZ;
  auto bufoff = 0;

  for (size_t i = 0; i < vblen; ++i) {
    auto blk = pg->bids[bidx];
    if (hashmap.find(blk) == hashmap.end()) {
      auto nwfrm = frm_alloc(blk);
      hashmap[blk] = nwfrm;
    }
    frameid_t frm = hashmap[blk];
    auto written = frm_read(frm, off, &((char *)buf)[bufoff], len);
    pg->offset_ += written;
    bufoff += written;
    len -= written;
    off = 0;
  }
}

auto Disk::DiskManager::frm_alloc(blockid_t bid) -> frameid_t {
  std::priority_queue<record_t, std::vector<record_t>, std::greater<record_t>> pq;

  for (size_t i = 0; i < FRME_NM; ++i) {
    pq.push(rcds[i]);
  }
  if (pq.top()->inuse_) {
    assert(0 && "bad alloc");
  }
  auto fid = pq.top()->fid_;
  auto &&p = hashmap.find(frms[fid]->bid_);
  if (p != hashmap.end()) {
    hashmap.erase(p);
  }
  pq.top()->release();
  frms[fid]->frm_drop();
  frms[fid]->frm_load(bid);
  return fid;
}

auto Disk::DiskManager::frm_release(frameid_t fid) -> void {

}

auto Disk::DiskManager::frm_write(frameid_t fid, size_t offset, void *buf, size_t len) -> size_t {
  rcds[fid]->access();
  auto off = frms[fid]->frm_write(offset, buf, len);
  rcds[fid]->release();
  return off;
}

auto Disk::DiskManager::frm_read(frameid_t fid, size_t offset, void *buf, size_t len) -> size_t {
  rcds[fid]->access();
  auto off = frms[fid]->frm_read(offset, buf, len);
  rcds[fid]->release();
  return off;
}