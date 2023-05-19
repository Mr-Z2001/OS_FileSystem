#ifndef __FILEFULLNAME_HPP__
#define __FILEFULLNAME_HPP__
#include <string>
#include <cassert>

class FileFullName
{
private:
  std::string _fileNameWithoutExtension;
  std::string _fileExtension;

  void splitFilename(const std::string &fullName, std::string &fileNameWithoutExtension, std::string &Extension);

public:
  FileFullName();
  FileFullName(std::string fileFullName);
  FileFullName(std::string fileNameWithoutExtension, std::string fileExtension);
  ~FileFullName();

  std::string getFileNameWithoutExtension() const;
  std::string getFileExtension() const;
  std::string getFullName() const;
  bool setFileNameWithoutExtension(std::string fileNameWithoutExtension);
  bool setFileExtension(std::string fileExtension);
  bool setFullName(std::string fileFullName);
};

#endif