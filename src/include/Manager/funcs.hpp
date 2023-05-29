#ifndef __FUNCS_HPP__
#define __FUNCS_HPP__

#include "Node.h"
#include <map>
#include <vector>

void save(std::vector<std::vector<int>> *, std::map<int, Node *> *);
void load(std::vector<std::vector<int>> *, std::map<int, Node *> *);

#endif