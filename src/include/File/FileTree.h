//
// Created by 86158 on 2023/5/23.
//

#ifndef FILESYSTEM_FILETREE_H
#define FILESYSTEM_FILETREE_H
#include "Disk/DiskManager.hpp"
#include "Identity.h"
#include "Node.h"
#include <map>
class FileTree {

public:
  std::vector<std::vector<int>> tree; // 邻接表，存着树的信息
  std::map<int, Node *> project;      // 记录映射关系
private:
  int index; //  给tree编号用的

  int current_directory; // 当前目录
  std::vector<std::string>
  split(std::string path,
        int type); // 将路径切开，返回每个目录名，例如path=/A/B/C return A,B,C  type=0(绝对) type=1(相对)
  bool is_path(std::string &filename);  // 文件名是一个路径
  void delete_file(Node *file);         // 删除文件
  void delete_directory(int directory); // 删除目录
  void tostring(int file, std::string &path);

  Disk::DiskManager *dmgr;

public:
  FileTree();
  void init();
  void addChild(int parent, Node *child_p);
  int find_node_by_name(std::string name);         // 按在当前目录下按文件名找到节点
  int find_node_by_path(std::string path);         // 按路径找到节点
  void create(Identity *id, std::string filename); // 创建文件
  void mkdir(Identity *id, bool m, int user_mode, int group_mode, bool parents, bool verbose,
             std::string directory); // 创建目录
  int get_current_directory();
  void cwd(char *cwd);
  void rm(Identity *id, bool force, bool interactive, bool recursive, bool directory, bool verbose,
          std::vector<std::string> filenames);
  std::string read(Identity *id, std::string filename, int block_num);

  void write(Identity *id, bool append, bool overwrite, std::string filename, std::string text);
  void cat(Identity *id, bool head, bool tail, bool more, bool number, int n, std::vector<std::string> filename);
  void chmod(Identity *id, bool recursive, int user_mode, int group_mode, std::vector<std::string> filename);
  void ls();

  void cd(std::string filename, char *newdir); //

  std::string full_name(std::string input);
  void set_index();

  void print();
};

#endif // FILESYSTEM_FILETREE_H