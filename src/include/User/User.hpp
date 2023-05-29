#ifndef __USER_HPP__
#define __USER_HPP__

#include <string>

class User {
private:
  int id;
  std::string username;
  std::string password;

public:
  User();
  User(int id, std::string username, std::string password);
  ~User();

  std::string getUsername();
  std::string getPassword();
  int getID();

  void setUsername(std::string username);
  void setPassword(std::string password);
  void setID(int id);
};

#endif