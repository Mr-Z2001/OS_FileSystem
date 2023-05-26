#include "UserFunctions.hpp"
#include <algorithm>
#include <iostream>
#include <string.h>
#include <vector>

// file type must be a file, not a directory
void create(Identity *id, std::string filename) {}

void pwd(std::string cwd) { std::cout << cwd << std::endl; }

void rm(Identity *id, bool force, bool interactive, bool recursive, bool directory, bool verbose,
        std::vector<std::string> filenames) {
  std::string gn = id->groupname;
  std::string un = id->username;
  std::vector<int> filedescs;
  for (auto s : filenames)
    filedescs.push_back(find(s));
  for (auto fd : filedescs)
    remove_from_disk(fd);
}

void open(Identity *id, std::string filename) {
  std::string gn = id->groupname;
  std::string un = id->username;
  int fd = find(filename);
  if (fd == -1) {
    std::cout << "File not found" << std::endl;
    return;
  }
  if (open_file(fd, gn, un) == -1)
    std::cout << "Permission denied" << std::endl;
  load_to_memory(fd);
}

void close(Identity *id, std::string filename) {
  std::string gn = id->groupname;
  std::string un = id->username;
  int fd = find(filename);
  if (fd == -1) {
    std::cout << "File not found" << std::endl;
    return;
  }
  if (close_file(fd, gn, un) == -1)
    std::cout << "Permission denied" << std::endl;
  save_to_disk(fd);
}

void cat(Identity *id, bool head, bool tail, bool more, bool number, int n, std::vector<std::string> filename) {
  std::string gn = id->groupname;
  std::string un = id->username;
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
  std::string gn = id->groupname;
  std::string un = id->username;
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

void mkdir(Identity *id, bool m, int mode, bool parents, bool verbose, std::string directory) {
  std::string gn = id->groupname;
  std::string un = id->username;
  if (parents) {
    // dfs mkdir
  } else {
    int fd = find(directory);
    if (fd != -1) {
      std::cout << "File already exists" << std::endl;
      return;
    }
    create_file(directory, gn, un, mode);
  }
  if (verbose) {
    std::cout << "Directory" << dir_name << " created " << std::endl;
  }
}

void cd(Identity *id, std::string directory, char *newDirectory) {
  std::string gn = id->groupname;
  std::string un = id->username;
  if (directory == "" || directory == "~") {
    // cd to (/home/username);
  }
  int fd = find(directory);
  if (fd == -1) {
    std::cout << "File not found" << std::endl;
    return;
  }
  if (cd_file(fd, gn, un) == -1)
    std::cout << "Permission denied" << std::endl;
  strcpy(newDirectory, directory);
}

void ls(Identity *id, bool all, bool almost_all, bool _directory, bool human_readable, bool inode, bool list,
        bool recursive, bool sortBySize, bool sortByTime, std::string directory) {
  std::string gn = id->groupname;
  std::string un = id->username;
  std::vector<std::string> files;
  std::string search_policy = "ignore_._started";
  if (all) {
    files.push_back(".");
    files.push_back("..");
  }
  if (all || almost_all)
    search_policy = "no_ignore_._started";
  if (recursive) {
    files.push_back(); // dfs
  } else {
    files = get_files(directory, search_policy);
  }
  if (list) {
    // construct a format of string as below
    // [d/f][rwxrwxrwx][username][groupname][size][time][filename/directoryname]
    // separate by '\t'
    // [d/f] if dir->'d', else null(show nothing)
    // [rwxrwxrwx] if no previlege->'---', else show previlege, e.g. [rwxr-xr-x]
    // [size] any format you want
    // [time] any format you want
    // [filename/directoryname]
  }
  if (sortBySize)
    std::sort(files.begin(), files.end(), [](std::string a, std::string b) { return get_size(a) < get_size(b); });
  if (sortByTime)
    std::sort(files.begin(), files.end(), [](std::string a, std::string b) { return get_time(a) < get_time(b); });
}

void mv(Identity *id, bool force, bool interactive, bool verbose, std::vector<std::string> source,
        std::string destination) {
  std::string gn = id->groupname;
  std::string un = id->username;

  for (auto file : source) {
    int fd = find(file);
    if (fd == -1) {
      std::cout << "File not found" << std::endl;
      return;
    }
    if (mv_file(fd, gn, un, destination) == -1)
      std::cout << "Permission denied" << std::endl;
  }

  if (verbose) {
    std::cout << "File moved" << std::endl;
  }
}

void cp(Identity *id, bool force, bool link, bool noclobber, bool update, bool verbose, std::vector<std::string> source,
        std::string destination) {
  std::string gn = id->groupname;
  std::string un = id->username;

  for (auto file : source) {
    int fd = find(file);
    if (fd == -1) {
      std::cout << "File not found" << std::endl;
      return;
    }
    if (cp_file(fd, gn, un, destination) == -1)
      std::cout << "Permission denied" << std::endl;
  }

  if (verbose) {
    std::cout << "File copied" << std::endl;
  }
}

void chmod(Identity *id, bool recursive, int mode, std::vector<std::string> filename) {
  std::string gn = id->groupname;
  std::string un = id->username;
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