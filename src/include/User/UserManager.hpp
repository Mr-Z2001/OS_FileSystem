#ifndef __USERMANAGER_HPP__
#define __USERMANAGER_HPP__

#include "User.hpp"
#include "UserGroup.hpp"
#include <map>
#include <vector>

class UserManager {
private:
  std::vector<int> loggedUsers;
  std::vector<int> users;
  std::vector<int> userGroups;
  // std::vector<User *> loggedUsers;
  // std::vector<User *> users;
  // std::vector<UserGroup *> userGroups;

  // maps<int, entity>
  std::map<int, User *> userMap;
  std::map<int, UserGroup *> userGroupMap;

private:
  int getUserIDByName(std::string username);
  int usercnt = 0;

public:
  UserManager();
  ~UserManager();

  void loadUserMap();
  void loadUserGroupMap();

  void saveUserMap();
  void saveUserGroupMap();

  bool login(std::string username, std::string hashed_password);
  bool logout(std::string username);
  bool registerUser(std::string username, std::string password);
  bool isLogged(std::string username);
  User *getUser(std::string username);
  std::vector<User *> getLoggedUsers();
  UserGroup *getUserGroup(User *user);
};

#endif