#ifndef __USERMANAGER_HPP__
#define __USERMANAGER_HPP__

#include "User.hpp"
#include "UserGroup.hpp"
#include <vector>

class UserManager
{
private:
  std::vector<User *> loggedUsers;
  std::vector<User *> users;
  std::vector<UserGroup *> userGroups;

public:
  UserManager();
  ~UserManager();

  bool login(std::string username, std::string password);
  bool logout(std::string username);
  bool registerUser(std::string username, std::string password);
  bool isLogged(std::string username);
  User *getUser(std::string username);
  std::vector<User *> getLoggedUsers();
};

#endif