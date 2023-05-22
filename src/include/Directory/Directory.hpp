#ifndef __DIRECTORY_HPP__
#define __DIRECTORY_HPP__

#include <string>
#include <vector>
#include "File.hpp"

class Directory : public File
{
private:
  int directoryID;
  std::string name;
  std::string absolutePath;
  std::vector<Directory *> subDirectories;
  std::vector<File *> files;

public:
  Directory();
  ~Directory();

  int getID();
  std::string getName();
  std::string getAbsolutePath();
  std::vector<Directory *> getSubDirectories();
  std::vector<File *> getFiles();

  void setID(int directoryID);
  void setName(std::string name);
  void setAbsolutePath(std::string absolutePath);
  void setSubDirectories(std::vector<Directory *> subDirectories);
  void setFiles(std::vector<File *> files);

  void addSubDirectory(Directory *subDirectory);
  void addFile(File *file);
};

#endif