#ifndef __USERGROUP_HPP__
#define __USERGROUP_HPP__

#include <string>
#include <vector>

#include "User.hpp"

class UserGroup {
private:
  int id;
  std::string name;
  std::vector<int> users;

public:
  UserGroup();
  UserGroup(int id, std::string name);
  ~UserGroup();

  int getId();
  std::string getName();
  std::vector<int> getUsers();

  void setId(int id);
  void setName(std::string name);
  void setUsers(std::vector<int> users);

  void addUser(int user);
  void removeUser(int user);
};

#endif