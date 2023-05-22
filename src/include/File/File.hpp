#ifndef __FILE_HPP__
#define __FILE_HPP__

#include "FileAttribute.hpp"
#include "FileContext.hpp"
#include "FileFullName.hpp"
#include "FilePermission.hpp"
#include "FileType.hpp"

class File
{
private:
  FileAttribute *arrtribute;
  FileContext *context;
  FileFullName *fullName;
  FilePermission *permission;
  FileType *type;

public:
  File();
  ~File();
};

#endif