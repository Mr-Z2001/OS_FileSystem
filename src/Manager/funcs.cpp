#include "funcs.hpp"
#include "utils.hpp"
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

void save(std::vector<std::vector<int>> *tree, std::map<int, Node *> *info) {
  std::ofstream treef, nodeInfof;
  treef.open("tree.csv");
  nodeInfof.open("nodeInfo.csv");

  // check
  assert(treef.is_open() && nodeInfof.is_open());

  // save tree info.
  treef << tree->size() << ',';
  size_t sz = 0;
  for (int i = 0; i < tree->size(); ++i)
    sz += tree->at(i).size();
  treef << sz << std::endl;
  for (int i = 0; i < tree->size(); ++i)
    for (int j = 0; j < tree->at(i).size(); ++j)
      treef << i << ',' << (tree->at(i))[j] << std::endl;

  // save node info
  for (auto it = info->begin(); it != info->end(); ++it) {
    Node *t = it->second;
    // non-iterables.
    // clang-format off
    nodeInfof << it->first << ','; // node index
    nodeInfof << t->fd << ',' 
              << t->name << ',' 
              << t->type << ',' 
              << t->length << ',' 
              << t->link_num << ','
              << t->create_time << ',' 
              << t->last_modify << ',' 
              << t->parent << ',';
    // clang-format on
    // group previleges.
    auto sz = t->group_privilege.size();
    nodeInfof << sz << ',';
    for (auto i : t->group_privilege)
      nodeInfof << i.first << ',' << i.second << ',';
    // user previleges
    sz = t->user_privilege.size();
    nodeInfof << sz << ',';
    for (auto i : t->user_privilege)
      nodeInfof << i.first << ',' << i.second << ',';
    // location
    sz = t->location.size();
    nodeInfof << sz << ',';
    for (auto i : t->location)
      nodeInfof << i << ',';
    // link
    sz = t->link.size();
    nodeInfof << sz;
    if (sz != 0)
      for (auto i : t->link)
        nodeInfof << ',' << i;
    nodeInfof << std::endl;
  }

  treef.close();
  nodeInfof.close();
}

void load(std::vector<std::vector<int>> *tree, std::map<int, Node *> *node_info) {
  std::ifstream treef, nodeInfof;
  std::stringstream ss;
  std::string line;
  std::string cell;
  treef.open("tree.csv");
  nodeInfof.open("nodeInfo.csv");
  // check
  assert(treef.is_open() && nodeInfof.is_open());

  // load tree (vector<vector<int>>)
  size_t n, m;
  std::getline(treef, line);
  ss.str(line);
  ss.clear();
  std::getline(ss, cell, ',');
  n = strToInt(cell);
  std::getline(ss, cell, ',');
  m = strToInt(cell);

  std::vector<int> v;
  tree->resize(n);
  while (std::getline(treef, line)) {
    ss.str(line);
    ss.clear();
    while (std::getline(ss, cell, ','))
      v.push_back(strToInt(cell));
    tree->at(v[0]).push_back(v[1]);
    v.clear();
  }

  // load nodeInfo (map)
  Node *t;
  std::vector<std::string> vs;
  int node_id;
  int i, cnt;
  while (std::getline(nodeInfof, line)) {
    i = 0;
    vs.clear();
    ss.str(line);
    ss.clear();
    t = new Node();
    while (std::getline(ss, cell, ','))
      vs.push_back(cell);
    node_id = strToInt(vs[0]);
    // non-iterables;
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
  treef.close();
  nodeInfof.close();
}