#ifndef __DISK_HPP__
#define __DISK_HPP__

#include <fstream>
#include <string>

class Disk
{
private:
  std::fstream disk;
  std::string path;

public:
  Disk(std::string path = "Disk0.bin");
  ~Disk();

  void write(int position, std::string data);
  std::string read(int position, int size);
  void clear();
};

#endif