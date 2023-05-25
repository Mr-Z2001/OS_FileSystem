#ifndef __FILEATTRIBUTE_HPP__
#define __FILEATTRIBUTE_HPP__

#include "FilePermission.hpp"
#include "User.hpp"
#include <chrono>
#include <cinttypes>
#include <string>

class FileAttribute {
public:
  // 8 bits - [LOCKED=128, TEMPORARY, RANDOMACCESS, BINARY, SAVE, SYSTEM, HIDDEN, READONLY=1]
  enum class TAG {
    READONLY = 1,
    HIDDEN = 2,
    SYSTEM = 4,
    SAVE = 8,
    BINARY = 16,
    RANDOMACCESS = 32,
    TEMPORARY = 64,
    LOCKED = 128
  };

private:
  FilePermission _permission;
  std::string _password;
  User *_creator;
  User *_owner;
  uint8_t _tag;
  std::chrono::system_clock::time_point _creationTime;
  std::chrono::system_clock::time_point _lastAccessTime;
  std::chrono::system_clock::time_point _lastModifiedTime;
  size_t _size;
  size_t _maxSize;

public:
  FileAttribute();
  FileAttribute(const FileAttribute &other);
  FileAttribute(FileAttribute &&other);
  FileAttribute &operator=(const FileAttribute &other);
  FileAttribute &operator=(FileAttribute &&other);
  ~FileAttribute();

  FilePermission getPermission() const;
  std::string getPassword() const;
  User *getCreator() const;
  User *getOwner() const;
  uint8_t getTag() const;
  std::chrono::system_clock::time_point getCreationTime() const;
  std::chrono::system_clock::time_point getLastAccessTime() const;
  std::chrono::system_clock::time_point getLastModifiedTime() const;
  size_t getSize() const;
  size_t getMaxSize() const;

  void setPermission(FilePermission permission);
  void setPassword(std::string password);
  void setCreator(User *creator);
  void setOwner(User *owner);
  void setTag(uint8_t tag);
  void enableTag(TAG tag);
  void disableTag(TAG tag);
  void setCreationTime(std::chrono::system_clock::time_point creationTime);
  void setLastAccessTime(std::chrono::system_clock::time_point lastAccessTime);
  void setLastModifiedTime(std::chrono::system_clock::time_point lastModifiedTime);
  void setSize(size_t size);
  void setMaxSize(size_t maxSize);
};

#endif