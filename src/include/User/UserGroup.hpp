#ifndef __USERGROUP_HPP__
#define __USERGROUP_HPP__

#include <string>
#include <vector>

#include "User.hpp"

class UserGroup
{
private:
  int id;
  std::string name;
  std::vector<User *> users;

public:
  UserGroup();
  UserGroup(int id, std::string name);
  ~UserGroup();

  int getId();
  std::string getName();
  std::vector<User *> getUsers();

  void setId(int id);
  void setName(std::string name);
  void setUsers(std::vector<User *> users);

  void addUser(User *user);
  void removeUser(User *user);
};

#endif