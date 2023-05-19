#ifndef __FILECONTEXT_HPP__
#define __FILECONTEXT_HPP__
#include <string>

class FileContext
{
private:
  std::string _fileContext;

public:
  FileContext();
  ~FileContext();
  std::string getFileContext() const;
  std::string getFileContext(int begin, int end);
  bool setFileContext(std::string fileContext);
};

#endif