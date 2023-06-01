#include "funcs.hpp"
#include "DiskDefs.hpp"
#include "UserFunctions.hpp"
#include "utils.hpp"

#include <cassert>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

void save(std::vector<std::vector<int>> *tree, std::map<int, Node *> *info) {
  Disk::blockid_t tree_bid = 3;
  Disk::blockid_t nodeInfo_bid = 2;
  char *tree_buf, *nodeInfo_buf;
  size_t buflen = Disk::PAGE_SIZ;

  // save tree info.
  std::string sz_str;
  sz_str = std::to_string(tree->size()) + ',';
  size_t sz = 0;
  for (int i = 0; i < tree->size(); ++i)
    sz += tree->at(i).size();
  sz_str += std::to_string(sz) + '\n';

  strcat(tree_buf, sz_str.c_str());

  sz_str.clear();
  for (int i = 0; i < tree->size(); ++i)
    for (int j = 0; j < tree->at(i).size(); ++j)
      sz_str += std::to_string(i) + ',' + std::to_string((tree->at(i))[j]) + '\n';

  strcat(nodeInfo_buf, sz_str.c_str());

  // save node info

  for (auto it = info->begin(); it != info->end(); ++it) {
    Node *t = it->second;
    // non-iterables.
    strcat(nodeInfo_buf, (std::to_string(it->first) + ",").c_str());
    strcat(nodeInfo_buf, (std::to_string(t->fd) + ",").c_str());
    strcat(nodeInfo_buf, (t->name + ",").c_str());
    strcat(nodeInfo_buf, (t->type + ",").c_str());
    strcat(nodeInfo_buf, (std::to_string(t->length) + ",").c_str());
    strcat(nodeInfo_buf, (std::to_string(t->link_num) + ",").c_str());
    strcat(nodeInfo_buf, (t->create_time + ",").c_str());
    strcat(nodeInfo_buf, (t->last_modify + ",").c_str());
    strcat(nodeInfo_buf, (std::to_string(t->parent) + ",").c_str());
    // group previleges.
    auto sz = t->group_privilege.size();
    strcat(nodeInfo_buf, (std::to_string(sz) + ",").c_str());
    for (auto i : t->group_privilege)
      strcat(nodeInfo_buf, (i.first + "," + std::to_string(i.second) + ",").c_str());
    // user previleges
    sz = t->user_privilege.size();
    strcat(nodeInfo_buf, (std::to_string(sz) + ",").c_str());
    for (auto i : t->user_privilege)
      strcat(nodeInfo_buf, (i.first + "," + std::to_string(i.second) + ",").c_str());
    // location
    sz = t->location.size();
    strcat(nodeInfo_buf, (std::to_string(sz) + ",").c_str());
    for (auto i : t->location)
      strcat(nodeInfo_buf, (std::to_string(i) + ",").c_str());
    // link
    sz = t->link.size();
    strcat(nodeInfo_buf, (std::to_string(sz) + ",").c_str());
    for (auto i : t->link)
      strcat(nodeInfo_buf, ("," + std::to_string(i)).c_str());
    strcat(nodeInfo_buf, "\n");
  }

  // write to disk
  ft->syswrite(tree_bid, tree_buf, buflen);
  ft->syswrite(nodeInfo_bid, nodeInfo_buf, buflen);

  // std::ofstream treef, nodeInfof;
  // treef.open("tree.csv");
  // nodeInfof.open("nodeInfo.csv");

  // // check
  // assert(treef.is_open() && nodeInfof.is_open());

  // // save tree info.
  // treef << tree->size() << ',';
  // size_t sz = 0;
  // for (int i = 0; i < tree->size(); ++i)
  //   sz += tree->at(i).size();
  // treef << sz << std::endl;
  // for (int i = 0; i < tree->size(); ++i)
  //   for (int j = 0; j < tree->at(i).size(); ++j)
  //     treef << i << ',' << (tree->at(i))[j] << std::endl;

  // // save node info
  // for (auto it = info->begin(); it != info->end(); ++it) {
  //   Node *t = it->second;
  //   // non-iterables.
  //   // clang-format off
  //       nodeInfof << it->first << ','; // node index
  //       nodeInfof << t->fd << ','
  //                 << t->name << ','
  //                 << t->type << ','
  //                 << t->length << ','
  //                 << t->link_num << ','
  //                 << t->create_time << ','
  //                 << t->last_modify << ','
  //                 << t->parent << ',';
  //   // clang-format on
  //   // group previleges.
  //   auto sz = t->group_privilege.size();
  //   nodeInfof << sz << ',';
  //   for (auto i : t->group_privilege)
  //     nodeInfof << i.first << ',' << i.second << ',';
  //   // user previleges
  //   sz = t->user_privilege.size();
  //   nodeInfof << sz << ',';
  //   for (auto i : t->user_privilege)
  //     nodeInfof << i.first << ',' << i.second << ',';
  //   // location
  //   sz = t->location.size();
  //   nodeInfof << sz << ',';
  //   for (auto i : t->location)
  //     nodeInfof << i << ',';
  //   // link
  //   sz = t->link.size();
  //   nodeInfof << sz;
  //   if (sz != 0)
  //     for (auto i : t->link)
  //       nodeInfof << ',' << i;
  //   nodeInfof << std::endl;
  // }

  // treef.close();
  // nodeInfof.close();
}
void load(std::vector<std::vector<int>> *tree, std::map<int, Node *> *node_info) {
  Disk::blockid_t tree_bid = 3;
  Disk::blockid_t nodeInfo_bid = 2;
  char *tree_buf, *nodeInfo_buf;
  size_t buflen = Disk::PAGE_SIZ;
  ft->sysread(tree_bid, tree_buf, buflen);
  ft->sysread(nodeInfo_bid, nodeInfo_buf, buflen);

  // tree
  int node_cnt, edge_cnt;
  std::vector<std::string> data = split(tree_buf, '\n');
  std::vector<std::string> vs = split(data[0], ',');
  node_cnt = strToInt(vs[0]);
  edge_cnt = strToInt(vs[1]);
  tree->resize(node_cnt);
  for (int i = 1; i <= edge_cnt; ++i) {
    vs = split(data[i], ',');
    tree->at(strToInt(vs[0])).push_back(strToInt(vs[1]));
  }

  // node-info
  Node *t;
  int node_id;
  int i, cnt;
  data = split(nodeInfo_buf, '\n');
  for (auto line : data) {
    i = 0;
    vs.clear();
    vs = split(line, ',');
    node_id = strToInt(vs[0]);
    // non-iterables;
    t = new Node();
    t->fd = strToInt(vs[1]);
    t->name = vs[2];
    t->type = vs[3];
    t->length = strToInt(vs[4]);
    t->link_num = strToInt(vs[5]);
    t->create_time = vs[6];
    t->last_modify = vs[7];
    t->parent = strToInt(vs[8]);
    // iterables;
    cnt = strToInt(vs[9]);
    i = 10;
    while (cnt--) {
      t->group_privilege.insert(std::make_pair(vs[i], strToInt(vs[i + 1])));
      i += 2;
    }
    cnt = strToInt(vs[i++]);
    while (cnt--) {
      t->user_privilege.insert(std::make_pair(vs[i], strToInt(vs[i + 1])));
      i += 2;
    }
    cnt = strToInt(vs[i++]);
    while (cnt--)
      t->location.push_back(strToInt(vs[i++]));
    cnt = strToInt(vs[i++]);
    while (cnt--)
      t->link.insert(strToInt(vs[i++]));
    node_info->insert(std::make_pair(node_id, t));
  }

  // std::ifstream treef, nodeInfof;
  // std::stringstream ss;
  // std::string line;
  // std::string cell;
  // treef.open("tree.csv");
  // nodeInfof.open("nodeInfo.csv");
  // // check
  // assert(treef.is_open() && nodeInfof.is_open());

  // // load nodeInfo (map)
  // Node *t;
  // std::vector<std::string> vs;
  // int node_id;
  // int i, cnt;
  // while (std::getline(nodeInfof, line)) {
  //   i = 0;
  //   vs.clear();
  //   ss.str(line);
  //   ss.clear();
  //   t = new Node();
  //   while (std::getline(ss, cell, ','))
  //     vs.push_back(cell);
  //   node_id = strToInt(vs[0]);
  //   // non-iterables;
  //   t->fd = strToInt(vs[1]);
  //   t->name = vs[2];
  //   t->type = vs[3];
  //   t->length = strToInt(vs[4]);
  //   t->link_num = strToInt(vs[5]);
  //   t->create_time = vs[6];
  //   t->last_modify = vs[7];
  //   t->parent = strToInt(vs[8]);
  //   // iterables;
  //   cnt = strToInt(vs[9]);
  //   i = 10;
  //   while (cnt--) {
  //     t->group_privilege.insert(std::make_pair(vs[i], strToInt(vs[i + 1])));
  //     i += 2;
  //   }
  //   cnt = strToInt(vs[i++]);
  //   while (cnt--) {
  //     t->user_privilege.insert(std::make_pair(vs[i], strToInt(vs[i + 1])));
  //     i += 2;
  //   }
  //   cnt = strToInt(vs[i++]);
  //   while (cnt--)
  //     t->location.push_back(strToInt(vs[i++]));
  //   cnt = strToInt(vs[i++]);
  //   while (cnt--)
  //     t->link.insert(strToInt(vs[i++]));
  //   node_info->insert(std::make_pair(node_id, t));
  // }

  // treef.close();
  // nodeInfof.close();
}