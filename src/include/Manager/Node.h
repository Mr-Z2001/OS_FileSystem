//
// Created by 86158 on 2023/5/23.
//

#ifndef FILESYSTEM_NODE_H
#define FILESYSTEM_NODE_H
/***
 * 树的结点
 */
#include "Identity.h"
#include <iostream>
#include <set>
#include <vector>

class Node {

public:
  int fd = -1;                                           // 文件描述符
  std::string name;                                      // 文件名称
  std::string type;                                      // 文件类型
  std::set<std::pair<std::string, int>> group_privilege; // group权限   name     rwx(read write exe)
  std::set<std::pair<std::string, int>> user_privilege;  // user权限   [string]->[int]
  std::vector<int> location;                             // 物理地址
  int length;                                            // 文件长度
  int link_num;                                          // 链接计数
  std::string create_time;                               // 创建时间
  std::string last_modify;                               // 最后一次修改时间
  std::set<int>
      link; // 链接（若本节点只被一个节点指向，则看成是parent，两个及以上则视为一个是parent，其他是link），改成了编号
  int parent; // 父节点,改成了编号

  Node();
  Node(std::string name);
  Node(std::string name, std::string type);
  Node(Identity *id, std::string filename);

  void addChild(Node *&child);                               // 增加子节点
  std::vector<std::pair<std::string, Node *>> getChildren(); // 得到孩子<文件名，指针>
  Node *getParent();                                         // 得到父节点
  std::string getName();                                     // 得到名字
  std::string getType();
  void setIndex(int i);
  int getIndex();
};

#endif // FILESYSTEM_NODE_H
