#include "FileFullName.hpp"
#include <algorithm>

void splitFilename(const std::string &fullName, std::string &fileNameWithoutExtension, std::string &Extension)
{
  size_t dotPos = fullName.find_last_of(".");
  fileNameWithoutExtension = fullName.substr(0, dotPos);
  Extension = fullName.substr(dotPos + 1);
}

FileFullName::FileFullName()
{
  _fileNameWithoutExtension = "";
  _fileExtension = "";
}

FileFullName::FileFullName(std::string fileFullName)
{
  std::string fn, e;
  splitFilename(fileFullName, fn, e);
  assert(fn != "");
  assert(e != "");
  _fileNameWithoutExtension = fn;
  _fileExtension = e;
}

FileFullName::FileFullName(std::string fileNameWithoutExtension, std::string fileExtension)
{
  assert(fileNameWithoutExtension != "");
  assert(fileExtension != "");
  _fileNameWithoutExtension = fileNameWithoutExtension;
  _fileExtension = fileExtension;
}

FileFullName::~FileFullName() {}

std::string FileFullName::getFileNameWithoutExtension() const
{
  return _fileNameWithoutExtension;
}

std::string FileFullName::getFileExtension() const
{
  return _fileExtension;
}

std::string FileFullName::getFullName() const
{
  return _fileNameWithoutExtension + "." + _fileExtension;
}

bool FileFullName::setFileNameWithoutExtension(std::string fileNameWithoutExtension)
{
  if (fileNameWithoutExtension == "")
    return false;
  _fileNameWithoutExtension = fileNameWithoutExtension;
  return true;
}

bool FileFullName::setFileExtension(std::string fileExtension)
{
  if (fileExtension == "")
    return false;
  _fileExtension = fileExtension;
  return true;
}

bool FileFullName::setFullName(std::string fileFullName)
{
  std::string fn, e;
  splitFilename(fileFullName, fn, e);
  if (fn == "" || e == "")
    return false;
  _fileNameWithoutExtension = fn;
  _fileExtension = e;
  return true;
}
