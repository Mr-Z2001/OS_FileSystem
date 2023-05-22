#include "Sector.hpp"
#include <exception>
#include <iostream>

Sector::Sector() {}
Sector::~Sector() {}

void Sector::write(int position, std::string str)
{
  for (int i = 0; i < str.length(); i++)
    try
    {
      arr.at(position + i) = str[i];
    }
    catch (const std::out_of_range &e)
    {
      std::cout << "Out of Range error: " << e.what() << '\n';
    }
}

std::string Sector::read(int position, std::size_t length)
{
  std::string str;
  for (int i = 0; i < length; i++)
    try
    {
      str += arr.at(position + i);
    }
    catch (const std::out_of_range &e)
    {
      std::cout << "Out of Range error: " << e.what() << '\n';
    }
  return str;
}

void Sector::clear()
{
  arr.fill(0);
}

std::size_t Sector::getSize()
{
  return size;
}