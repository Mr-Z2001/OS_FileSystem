#include "User.hpp"

User::User() {}
User::User(std::string username, std::string password)
{
  this->username = username;
  this->password = password;
}

User::~User() {}

std::string User::getUsername()
{
  return username;
}

std::string User::getPassword()
{
  return password;
}

void User::setUsername(std::string username)
{
  this->username = username;
}

void User::setPassword(std::string password)
{
  this->password = password;
}
