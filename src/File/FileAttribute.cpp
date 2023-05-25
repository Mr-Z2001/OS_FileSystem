#include "FileAttribute.hpp"

FileAttribute::FileAttribute() {
  _permission = FilePermission();
  _password = "";
  _creator = nullptr;
  _owner = nullptr;
  _tag = 0;
  _creationTime = std::chrono::system_clock::now();
  _lastAccessTime = std::chrono::system_clock::now();
  _lastModifiedTime = std::chrono::system_clock::now();
  _size = 0;
  _maxSize = 0;
}

FileAttribute::FileAttribute(const FileAttribute &other) {
  _permission = other._permission;
  _password = other._password;
  _creator = other._creator;
  _owner = other._owner;
  _tag = other._tag;
  _creationTime = other._creationTime;
  _lastAccessTime = other._lastAccessTime;
  _lastModifiedTime = other._lastModifiedTime;
  _size = other._size;
  _maxSize = other._maxSize;
}

FileAttribute::FileAttribute(FileAttribute &&other) {
  _permission = other._permission;
  _password = other._password;
  _creator = other._creator;
  _owner = other._owner;
  _tag = other._tag;
  _creationTime = other._creationTime;
  _lastAccessTime = other._lastAccessTime;
  _lastModifiedTime = other._lastModifiedTime;
  _size = other._size;
  _maxSize = other._maxSize;
}

FileAttribute &FileAttribute::operator=(const FileAttribute &other) {
  _permission = other._permission;
  _password = other._password;
  _creator = other._creator;
  _owner = other._owner;
  _tag = other._tag;
  _creationTime = other._creationTime;
  _lastAccessTime = other._lastAccessTime;
  _lastModifiedTime = other._lastModifiedTime;
  _size = other._size;
  _maxSize = other._maxSize;
  return *this;
}

FileAttribute &FileAttribute::operator=(FileAttribute &&other) {
  _permission = other._permission;
  _password = other._password;
  _creator = other._creator;
  _owner = other._owner;
  _tag = other._tag;
  _creationTime = other._creationTime;
  _lastAccessTime = other._lastAccessTime;
  _lastModifiedTime = other._lastModifiedTime;
  _size = other._size;
  _maxSize = other._maxSize;
  return *this;
}

FileAttribute::~FileAttribute() {}

FilePermission FileAttribute::getPermission() const { return _permission; }

std::string FileAttribute::getPassword() const { return _password; }

User *FileAttribute::getCreator() const { return _creator; }

User *FileAttribute::getOwner() const { return _owner; }

uint8_t FileAttribute::getTag() const { return _tag; }

std::chrono::system_clock::time_point FileAttribute::getCreationTime() const { return _creationTime; }

std::chrono::system_clock::time_point FileAttribute::getLastAccessTime() const { return _lastAccessTime; }

std::chrono::system_clock::time_point FileAttribute::getLastModifiedTime() const { return _lastModifiedTime; }

size_t FileAttribute::getSize() const { return _size; }

size_t FileAttribute::getMaxSize() const { return _maxSize; }

void FileAttribute::setPermission(FilePermission permission) { _permission = permission; }

void FileAttribute::setPassword(std::string password) { _password = password; }

void FileAttribute::setCreator(User *creator) { _creator = creator; }

void FileAttribute::setOwner(User *owner) { _owner = owner; }

void FileAttribute::setTag(uint8_t tag) { _tag = tag; }

void FileAttribute::enableTag(TAG tag) { _tag |= static_cast<uint8_t>(tag); }

void FileAttribute::disableTag(TAG tag) { _tag &= ~static_cast<uint8_t>(tag); }

void FileAttribute::setCreationTime(std::chrono::system_clock::time_point creationTime) {
  _creationTime = creationTime;
}

void FileAttribute::setLastAccessTime(std::chrono::system_clock::time_point lastAccessTime) {
  _lastAccessTime = lastAccessTime;
}

void FileAttribute::setLastModifiedTime(std::chrono::system_clock::time_point lastModifiedTime) {
  _lastModifiedTime = lastModifiedTime;
}

void FileAttribute::setSize(size_t size) { _size = size; }

void FileAttribute::setMaxSize(size_t maxSize) { _maxSize = maxSize; }
