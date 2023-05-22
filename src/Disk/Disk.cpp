#include "Disk.hpp"

Disk::Disk(std::string path)
{
  this->path = path;
  this->disk.open(path, std::ios::in | std::ios::out | std::ios::binary);
}

Disk::~Disk() { this->disk.close(); }

void Disk::write(int position, std::string data)
{
  this->disk.seekp(position);
  this->disk.write(data.c_str(), data.size());
}

std::string Disk::read(int position, int size)
{
  char *buffer = new char[size];
  this->disk.seekg(position);
  this->disk.read(buffer, size);
  std::string data(buffer, size);
  delete[] buffer;
  return data;
}

void Disk::clear()
{
  this->disk.seekp(0);
  this->disk.write("", 1);
}
