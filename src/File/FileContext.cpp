#include "FileContext.hpp"

FileContext::FileContext() { _fileContext = ""; }

FileContext::~FileContext() {}

std::string FileContext::getFileContext() const { return _fileContext; }

std::string FileContext::getFileContext(int begin, int end) { return _fileContext.substr(begin, end - begin + 1); }

bool FileContext::setFileContext(std::string fileContext) {
  if (fileContext == "")
    return false;
  _fileContext = fileContext;
  return true;
}