//
// Created by 86158 on 2023/5/23.
//

#include "FileTree.h"
#include "DiskDefs.hpp"
#include "funcs.hpp"
#include <string.h>

FileTree::FileTree() {
  index = 0;
  current_directory = 0;
}

void FileTree::addChild(int parent, Node *child_p) {
  index++;
  child_p->parent = parent;
  tree[parent].push_back(index);
  project.insert({index, child_p});
  tree.push_back({});
}
void FileTree::init() {

  //   Node *root = new Node("/", "directory");
  //   project.insert({0, root});
  //   tree.push_back({});

  //   Node *bin = new Node("bin", "directory");
  //   Node *user = new Node("user", "directory");
  //   Node *home = new Node("home", "directory");
  //   addChild(0, bin);
  //   addChild(0, user);
  //   addChild(0, home);

  load(&tree, &project);
  set_index();
  current_directory = 0;

  std::vector<int> allblk;
  for (auto &&fp : project) {
    allblk.insert(allblk.end(), fp.second->location.begin(), fp.second->location.end());
  }
  dmgr = new Disk::DiskManager("hdd.bin", allblk);
}
int FileTree::get_current_directory() { return current_directory; }

// 在当前目录根据名字找到node
int FileTree::find_node_by_name(std::string name) {

  auto node = project.find(current_directory);

  if (node->second->getName() == name && node->second->fd > 0)
    return current_directory;
  for (auto i : tree[current_directory]) {
    if (project.find(i)->second->getName() == name && project.find(i)->second->fd > 0)
      return i;
  }
  return -1; // 没找到
}

bool FileTree::is_path(std::string &filename) {
  int k = filename.find('/');
  if (k >= 0)
    return true;
  return false;
}

void FileTree::syswrite(Disk::blockid_t bid, char *buf, size_t buflen) {
  assert(0 <= bid && 63 >= bid);
  dmgr->blk_write(bid, buf, buflen);
}

void FileTree::sysread(Disk::blockid_t bid, char *buf, size_t buflen) {
  assert(0 <= bid && 63 >= bid);
  dmgr->blk_read(bid, buf, buflen);
}

std::vector<std::string> FileTree::split(std::string path, int type = 0) {
  if (path[0] == '/')
    type = 0;
  else
    type = 1;
  std::vector<std::string> res;

  if (type == 0) // 绝对路径
  {
    int pos = path.find('/', 1);
    auto word = path.substr(1, pos - 1);
    path = path.substr(pos + 1);
    res.push_back(word);
    if (pos < 0)
      return res;
  } else // 相对路径
  {
    int pos = path.find('/');
    auto word = path.substr(0, pos);
    path = path.substr(pos + 1);
    res.push_back(word);
    if (pos < 0)
      return res;
  }

  while (path.size() > 0) {
    int pos = path.find('/');
    if (pos < 0)
      break;
    auto word = path.substr(0, pos);
    res.push_back(word);
    path = path.substr(pos + 1);
  }
  res.push_back(path);
  return res;
}
int FileTree::find_node_by_path(std::string path) // 按路径查找
{
  std::vector<std::string> words;
  if (path[0] == '/') // 绝对路径
  {
    current_directory = 0;
    words = split(path, 0); // 按绝对路径切片
  } else {
    words = split(path, 1); // 按相对路径切片
  }

  for (int i = 0; i < words.size(); ++i) {
    auto curdir = current_directory;
    current_directory = find_node_by_name(words[i]);
    if (current_directory == -1) {
      current_directory = curdir;
      return -1;
    }
  }
  return current_directory;
}
void FileTree::create(Identity *id, std::string filename) {
  if (!is_path(filename)) {
    auto curdir = current_directory;
    if (find_node_by_name(filename) != -1) {
      current_directory = curdir;
      std::cout << "file duplicated" << std::endl;
      return;
    }
    Node *file = new Node(id, filename);
    file->location.push_back(dmgr->blk_alloc(1)[0]);
    addChild(current_directory, file);
  } else {
    std::vector<std::string> words;
    if (filename[0] == '/') {
      current_directory = 0;
      words = split(filename, 0); // 按绝对路径切片
      for (int i = 0; i < words.size() - 1; ++i) {
        current_directory = find_node_by_name(words[i]);
        if (current_directory == -1) {
          std::cout << "error path" << std::endl; // 没找到目录
          current_directory = 0;
          return;
        }
      }
      Node *newfile = new Node(id, words[words.size() - 1]);
      newfile->location.push_back(dmgr->blk_alloc(1)[0]);
      addChild(current_directory, newfile);
    }
  }
}
void FileTree::cwd(char *cwd) {
  std::string pwd = project.find(current_directory)->second->name;
  strncpy(cwd, pwd.data(), pwd.size());
}

void FileTree::delete_file(Node *file) // 删除一个文件
{

  dmgr->blk_release(file->location); // 释放硬盘空间
  auto parent = file->getParent();
  for (int i = 0; i < tree[parent].size(); ++i) {
    auto kk = project.find(tree[parent][i])->second;
    if (kk == file) {
      kk->fd = -1;
      tree[parent].erase(tree[parent].begin() + i); // 修改邻接表
      kk->parent = -1;
    }
  }
}
void FileTree::delete_directory(int directory) {
  auto direc_p = project.find(directory)->second;
  if (direc_p->getType() == "directory") {
    if (tree[directory].size() == 0) // 空目录
    {
      auto parent = direc_p->getParent();
      for (int i = 0; i < tree[parent].size(); ++i) {
        auto child = project.find(tree[parent][i]);
        if (child->first == directory) {
          tree[parent].erase(tree[parent].begin() + i);
          child->second->fd = -1;
        }
      }

      //  delete direc_p;
    } else // 不是空目录
    {
      for (auto i : tree[directory]) {
        delete_directory(i);
      }
      if (tree[directory].size() == 0) // 空目录
      {
        auto parent = direc_p->getParent();
        for (int i = 0; i < tree[parent].size(); ++i) {
          auto child = project.find(tree[parent][i]);
          if (child->first == directory) {
            tree[parent].erase(tree[parent].begin() + i);
            child->second->fd = -1;
          }
        }

        //  delete direc_p;
      }
    }
  } else if (direc_p->getType() == "file") {
    delete_file(direc_p);
  }
}
void FileTree::mkdir(Identity *id, bool m, int user_mode, int group_mode, bool parents, bool verbose,
                     std::string directory) {
  if (!is_path(directory)) // 当前目录下建立目录
  {
    Node *file;
    if (m) {
      file = new Node(id, user_mode, group_mode, directory);
    } else {
      file = new Node(id, 111, 111, directory);
    }
    addChild(current_directory, file);
    if (verbose) {
      std::cout << "creating directory....." << std::endl;
    }
  } else {
    std::vector<std::string> words;
    if (directory[0] == '/') // 绝对路径
    {
      current_directory = 0;
      words = split(directory, 0); // 按绝对路径切片
      for (int i = 0; i < words.size(); ++i) {
        int temp = current_directory;
        current_directory = find_node_by_name(words[i]);
        if (current_directory != -1) {
          temp = current_directory;
        }
        if (current_directory == -1) {
          if (!parents) {
            std::cout << "can not create multi-directory" << std::endl; // 没找到目录
            current_directory = temp;
            return;
          } else {
            Node *newfile;
            if (m) {
              newfile = new Node(id, user_mode, group_mode, words[i]);
            } else {
              newfile = new Node(id, 111, 111, words[i]);
            }
            current_directory = temp;
            addChild(current_directory, newfile);
            current_directory = find_node_by_name(words[i]);
          }
        }
      }

    } else // 相对路径
    {
      words = split(directory, 1); // 按相对路径切片
      for (int i = 0; i < words.size(); ++i) {
        int temp = current_directory;
        current_directory = find_node_by_name(words[i]);
        if (current_directory != -1) {
          temp = current_directory;
        }
        if (current_directory == -1) {
          if (!parents) {
            std::cout << "can not create multi-directory" << std::endl; // 没找到目录
            current_directory = temp;
            return;
          } else {
            Node *newfile;
            if (m) {
              newfile = new Node(id, user_mode, group_mode, words[i]);
            } else {
              newfile = new Node(id, 111, 111, words[i]);
            }
            current_directory = temp;
            addChild(current_directory, newfile);
            current_directory = find_node_by_name(words[i]);
          }
        }
      }
    }
  }
}
void FileTree::rm(Identity *id, bool force, bool interactive, bool recursive, bool directory, bool verbose,
                  std::vector<std::string> filenames) {
  if(recursive && force && filenames[0] == "/")
  {
    delete_directory(0);
  }
  auto temp_file = current_directory;
  for (auto file : filenames) {
    current_directory = temp_file;
    int file_int;
    if (is_path(file)) {
      file_int = find_node_by_path(file);
    } else {
      file_int = find_node_by_name(file);
    }
    if (file_int == -1) // 没找到文件
    {
      if (!force) {
        std::cout << "error filename" << std::endl;
        current_directory = temp_file;
        return;
      }
    } else { // 找到文件

      auto file_pointer = project.find(file_int)->second;
      if (file_pointer->getType() == "file") { // 是纯文件
        std::string receiver = "yes";
        if (interactive) {
          std::cout << "remove " << file_pointer->name << " ?(y/n)" << std::endl;
          std::cin >> receiver;
        }

        if (receiver[0] == 'y') {
          delete_file(file_pointer);
          if (verbose) {
            std::cout << "removing " << file_pointer->name << "......" << std::endl;
          }
        }

      } else if (file_pointer->getType() == "directory") { // 删除目录
        if (recursive) {
          delete_directory(file_int);
        } else {
          std::cout << "can not delete directory" << std::endl;
          return;
        }
      }
    }
  }
}
std::string FileTree::read(Identity *id, std::string filename, int block_num) {
  auto curdir = current_directory;
  auto file = find_node_by_path(filename);

  if (file == -1) {
    std::cout << "file no exist" << std::endl;
    return "";
  } else {
    auto file_p = project.find(file)->second;
    auto gp = file_p->group_privilege;
    auto up = file_p->user_privilege;
    bool gp_permission = false;
    bool up_permission = false;
    for (auto i : gp) {
      if (i.first == id->groupname && i.second >= 100) {
        gp_permission = true;
      }
    }
    for (auto i : up) {
      if (i.first == id->username && i.second >= 100) {
        up_permission = true;
      }
    }
    if (gp_permission && up_permission) {
      std::string res = "";
      for (int i = 0; i < block_num; ++i) {
        auto bid = file_p->location[i];
        char buf[Disk::PAGE_SIZ];
        auto len = Disk::PAGE_SIZ;
        dmgr->blk_read(bid, (void *)buf, len);
        res += {buf};
      }
      current_directory = curdir;
      return res;
    } else {
      std::cout << "no permission" << std::endl;
      current_directory = curdir;
      return "";
    }
  }
  current_directory = curdir;
}

// 1行32个字符，1块4096有128行
void FileTree::cat(Identity *id, bool head, bool tail, bool more, bool number, int n,
                   std::vector<std::string> filename) {
  if (head) // 就这样吧 累了
  {
    int block_num = n / 129 + 1;
    auto res = read(id, filename[0], block_num);
    std::cout << res << std::endl;
  }
}

void FileTree::cd(std::string filename, char *newdir) {
  if (filename == "..") {
    if (current_directory == 0) {
      return;
    }

    auto p = project.find(current_directory)->second->parent;
    current_directory = p;
    strncpy(newdir, (char *)project.find(current_directory)->second->name.data(), 128);
    return;
  }
  if (filename == "/") {
    current_directory = 0;
    strncpy(newdir, "/", 128);
    return;
  }
  auto temp = current_directory;
  current_directory = find_node_by_path(filename);
  if (current_directory == -1) {
    std::cout << "error path" << std::endl;
    current_directory = temp;
    return;
  }
  strncpy(newdir, (char *)project.find(current_directory)->second->name.data(), 128);
  return;
}

void FileTree::write(Identity *id, bool append, bool overwrite, std::string filename, std::string text) {
  auto curdir = current_directory;
  auto file = find_node_by_path(filename);

  if (file == -1) {
    std::cout << "file no exist" << std::endl;
    return;
  }

  auto file_p = project.find(file)->second;
  auto gp = file_p->group_privilege;
  auto up = file_p->user_privilege;
  bool gp_permission = false;
  bool up_permission = false;
  for (auto i : gp) {
    if (i.first == id->groupname && (i.second == 111 || i.second == 110 || i.second == 011 || i.second == 010)) {
      gp_permission = true;
    }
  }
  for (auto i : up) {
    if (i.first == id->username && (i.second == 111 || i.second == 110 || i.second == 011 || i.second == 010)) {
      up_permission = true;
    }
  }
  if (gp_permission && up_permission) {

    if (append) {
      text = read(id, filename, file_p->location.size()) + text;
      overwrite = true;
    }

    if (overwrite) {
      auto textlen = text.size();
      auto text_blk_num = textlen / Disk::PAGE_SIZ + !!(textlen % Disk::PAGE_SIZ);
      auto cur_blk_num = file_p->location.size();
      if (cur_blk_num < text_blk_num) {
        auto extra_blk_num = text_blk_num - cur_blk_num;
        auto &&newblks = dmgr->blk_alloc(extra_blk_num);
        file_p->location.insert(file_p->location.end(), newblks.begin(), newblks.end());
      }
      file_p->length = textlen;
      auto cur_wrt_len = 0;

      for (int i = 0; i < text_blk_num; ++i) {
        auto cur_text = text.substr(cur_wrt_len, Disk::PAGE_SIZ);
        if (text.size() > Disk::PAGE_SIZ) {
          auto buf = cur_text.data();
          auto len = Disk::PAGE_SIZ;
          dmgr->blk_write(file_p->location[i], (void *)buf, len);
          cur_wrt_len += Disk::PAGE_SIZ;
        } else {
          auto buf = cur_text.data();
          auto len = cur_text.size();
          char tmpbuf[4096] = {0};
          memcpy(tmpbuf, buf, len);
          dmgr->blk_write(file_p->location[i], (void *)tmpbuf, Disk::PAGE_SIZ);
        }
      }
    }

    // auto bid = file_p->location[0];
    // auto buf = text.data();

    // auto len = Disk::PAGE_SIZ;
    // dmgr->blk_write(bid, (void *)buf, len);

    current_directory = curdir;
    return;
  }

  current_directory = curdir;
  std::cout << "no permission" << std::endl;
}

void FileTree::tostring(int file, std::string &path) {
  auto node = project.find(file)->second;

  path = path + "/" + node->getName();
  int length = path.size();
  if (path == "//")
    std::cout << "/" << std::endl;
  else {
    if (path.substr(0, 2) == "//")
      std::cout << path.substr(2) << std::endl;
    else
      std::cout << path << std::endl;
  }
  if (tree[file].size() == 0)
    return;
  for (int i = 0; i < tree[file].size(); ++i) {
    tostring(tree[file][i], path);
    path = path.substr(0, length);
  }
}

void FileTree::ls(std::string paths) {
  int last_file;
  if (paths == "") {
    last_file = current_directory;
  } else if (paths == "/") {
    auto index = 0;
    std::string path = "";
    tostring(index, path);
    return;
  } else if (is_path(paths)) {
    last_file = find_node_by_path(paths);
  } else {
    last_file = find_node_by_name(paths);
  }

  std::string path = "";
  tostring(last_file, path);
}

void FileTree::chmod(Identity *id, bool recursive, int user_mode, int group_mode, std::vector<std::string> filename) {
  auto curdir = current_directory;
  for (auto i : filename) {
    if (is_path(i)) {
      current_directory = find_node_by_path(i);
      auto abc = project.find(current_directory)->second;
      abc->group_privilege.insert({id->groupname, group_mode});
      abc->user_privilege.insert({id->username, user_mode});
    } else {
      current_directory = find_node_by_name(i);
      auto abc = project.find(current_directory)->second;
      abc->group_privilege.insert({id->groupname, group_mode});
      abc->user_privilege.insert({id->username, user_mode});
    }
  }
  current_directory = curdir;
}
void FileTree::print() {
  for (int i = 0; i < tree.size(); ++i) {
    std::cout << project.find(i)->second->getName() << " [" << i << "]:";
    for (int j = 0; j < tree[i].size(); ++j) {
      std::cout << tree[i][j] << " ";
    }
    std::cout << std::endl;
  }
}
std::string FileTree::full_name(std::string input) {
  if (input == "/") {
    return input;
  }

  if (input.size() > 3 && input.substr(0, 3) == "../") // 回上一级
  {
    input = input.substr(3);
  }
  if (input.size() > 2 && input.substr(0, 2) == "./") {
    input = input.substr(2);
  }
  auto temp = current_directory;
  std::string res = "";
  auto words = split(input);
  auto first_file = words[0];
  Node *node;
  for (auto i : project) {
    if (i.second->name == first_file) {
      node = i.second;
      break;
    }
  }

  std::vector<std::string> files;
  while (node->name != "/") {
    res = '/' + node->name + res;
    node = project.find(node->parent)->second;
  }
  for (int i = 1; i < words.size(); ++i) {
    res += '/' + words[i];
  }
  return res;
}

void FileTree::set_index() { this->index = this->tree.size() - 1; }