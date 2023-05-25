#ifndef __USER_HPP__
#define __USER_HPP__

#include <string>

class User {
private:
  std::string username;
  std::string password;

public:
  User();
  User(std::string username, std::string password);
  ~User();

  std::string getUsername();
  std::string getPassword();

  void setUsername(std::string username);
  void setPassword(std::string password);
};

#endif