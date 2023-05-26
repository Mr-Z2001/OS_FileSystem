#include "UserFunctions.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

void create(Identity id, std::string filename, std::string type) {}

void pwd(std::string cwd) { std::cout << cwd << std::endl; }

void rm(Identity id, bool force, bool interactive, bool recursive, bool directory, bool verbose,
        std::vector<std::string> filenames) {
  std::string gn = id.groupname;
  std::string un = id.username;
  std::vector<int> filedescs;
  for (auto s : filenames)
    filedescs.push_back(find(s));
  for (auto fd : filedescs)
    remove_from_disk(fd);
}

void open(Identity id, std::string filename) {
  std::string gn = id.groupname;
  std::string un = id.username;
  int fd = find(filename);
  if (fd == -1) {
    std::cout << "File not found" << std::endl;
    return;
  }
  if (open_file(fd, gn, un) == -1)
    std::cout << "Permission denied" << std::endl;
  load_to_memory(fd);
}

void close(Identity id, std::string filename) {
  std::string gn = id.groupname;
  std::string un = id.username;
  int fd = find(filename);
  if (fd == -1) {
    std::cout << "File not found" << std::endl;
    return;
  }
  if (close_file(fd, gn, un) == -1)
    std::cout << "Permission denied" << std::endl;
  save_to_disk(fd);
}

void cat(Identity id, bool head, bool tail, bool more, bool number, int n, std::vector<std::string> filename) {
  std::string gn = id.groupname;
  std::string un = id.username;
  std::vector<std::string> tot;
  for (auto file : filename) {
    int fd = find(file);
    std::vector<std::string> f = get_contents(fd);
    std::merge(tot.begin(), tot.end(), f.begin(), f.end(), tot);
  }
  if (number) {
    if (head)
      for (int line = 0; line < n; ++line)
        std::cout << line + 1 << '\t' << tot[line] << std::endl;
    else if (tail)
      for (int line = tot.size() - n; line < n; ++line)
        std::cout << line + 1 << '\t' << tot[line] << std::endl;
    else {
      char c;
      int line_cnt = 0;
      int step = 20;
      do {
        for (; line_cnt < tot.size(); ++line_cnt) {
          std::cout << line_cnt + 1 << '\t' << tot[line_cnt] << std::endl;
        }
        line_cnt += step;
        c = getch();
      } while (1)
    }
  }
}

void write(Indentity id, bool append, bool overwrite, std::string filename, std::string text) {
  std::string gn = id.groupname;
  std::string un = id.username;
  int fd = find(filename);
  if (fd == -1) {
    std::cout << "File not found" << std::endl;
    return;
  }
  if (append)
    append_to_file(fd, text);
  else if (overwrite)
    overwrite_to_file(fd, text);
}

void mkdir(Identity id, bool m, bool parents, bool verbose, std::string directory) {}

void chmod(Identity id, bool recursive, int mode, std::vector<std::string> filename) {
  std::string gn = id.groupname;
  std::string un = id.username;
  if (recursive) {
    // dfs chmod
  } else {
    for (auto file : filename) {
      int fd = find(file);
      if (fd == -1) {
        std::cout << "File not found" << std::endl;
        return;
      }
      if (chmod_file(fd, gn, un, mode) == -1)
        std::cout << "Permission denied" << std::endl;
    }
  }
}