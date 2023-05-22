#include "Directory.hpp"

Directory::Directory() {}
Directory::~Directory() {}

int Directory::getID()
{
  return directoryID;
}

std::string Directory::getName()
{
  return name;
}

std::string Directory::getAbsolutePath()
{
  return absolutePath;
}

std::vector<Directory *> Directory::getSubDirectories()
{
  return subDirectories;
}

std::vector<File *> Directory::getFiles()
{
  return files;
}

void Directory::setID(int directoryID)
{
  this->directoryID = directoryID;
}

void Directory::setName(std::string name)
{
  this->name = name;
}

void Directory::setAbsolutePath(std::string absolutePath)
{
  this->absolutePath = absolutePath;
}

void Directory::setSubDirectories(std::vector<Directory *> subDirectories)
{
  this->subDirectories = subDirectories;
}

void Directory::setFiles(std::vector<File *> files)
{
  this->files = files;
}

void Directory::addSubDirectory(Directory *subDirectory)
{
  subDirectories.push_back(subDirectory);
}

void Directory::addFile(File *file)
{
  files.push_back(file);
}
