#ifndef __FILEMANAGER_HPP__
#define __FILEMANAGER_HPP__

#include "File.hpp"
#include "User.hpp"

class FileManager {
private:
  User *user;
  File *file;
  std::string path;

public:
  FileManager();
  ~FileManager();

  void createFile();
};

#endif