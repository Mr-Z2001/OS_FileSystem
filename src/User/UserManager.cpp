#include "UserManager.hpp"
#include "FileTree.h"
// #include "cryptopp/sha.h"
#include "DiskDefs.hpp"
#include "UserFunctions.hpp"
#include "utils.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>

extern FileTree *ft;

void UserManager::loadUserMap() {
  Disk::blockid_t bid = 0;
  char *buf;
  size_t buflen = Disk::PAGE_SIZ;
  ft->sysread(bid, buf, buflen);
  std::string str(buf);
  std::vector<std::string> data;
  data = split(str, '\n');
  for (int i = 0; i < data.size(); ++i) {
    auto p = split(data[i], ',');
    User *u = new User;
    u->setID(std::stod(p[0]));
    u->setUsername(p[1]);
    u->setPassword(p[2]);
    userMap.insert(std::pair<int, User *>(u->getID(), u));
    users.push_back(u->getID());
    usercnt++;
  }

  // std::ifstream f;
  // f.open("userdata.csv");
  // assert(f.is_open());
  // std::string line;
  // while (std::getline(f, line)) {
  //   std::vector<std::string> data;
  //   std::regex pattern(",");
  //   std::copy(std::sregex_token_iterator(line.begin(), line.end(), pattern, -1), std::sregex_token_iterator(),
  //             std::back_inserter(data));
  //   User *u = new User;
  //   u->setID(std::stod(data[0]));
  //   u->setUsername(data[1]);
  //   u->setPassword(data[2]);
  //   userMap.insert(std::pair<int, User *>(u->getID(), u));
  //   users.push_back(u->getID());
  //   usercnt++;
  // }
  // f.close();
}

void UserManager::loadUserGroupMap() {

  Disk::blockid_t bid = 1;
  char *buf;
  size_t buflen = Disk::PAGE_SIZ;
  ft->sysread(bid, buf, buflen);
  std::string str(buf);
  std::vector<std::string> data;
  data = split(str, '\n');
  for (int i = 0; i < data.size(); ++i) {
    auto p = split(data[i], ',');
    UserGroup *ug = new UserGroup;
    ug->setId(std::stod(p[0]));
    ug->setName(p[1]);
    for (int i = 3; i < p.size(); i++) {
      int u = std::stod(p[i]);
      ug->addUser(u);
    }
    userGroupMap.insert(std::pair<int, UserGroup *>(ug->getId(), ug));
    userGroups.push_back(ug->getId());
  }

  // std::ifstream f;
  // f.open("userGroupData.csv");
  // assert(f.is_open());
  // std::string line;
  // while (std::getline(f, line)) {
  //   std::vector<std::string> data;
  //   std::regex pattern(",");
  //   std::copy(std::sregex_token_iterator(line.begin(), line.end(), pattern, -1), std::sregex_token_iterator(),
  //             std::back_inserter(data));
  //   UserGroup *ug = new UserGroup;
  //   ug->setId(std::stod(data[0]));
  //   ug->setName(data[1]);
  //   for (int i = 2; i < data.size(); i++) {
  //     int u = std::stod(data[i]);
  //     ug->addUser(u);
  //   }
  //   userGroupMap.insert(std::pair<int, UserGroup *>(ug->getId(), ug));
  //   userGroups.push_back(ug->getId());
  // }
  // f.close();
}

UserManager::UserManager() {
  loadUserMap();
  loadUserGroupMap();
}
UserManager::~UserManager() {
  for (auto i : userMap)
    delete i.second;
  for (auto i : userGroupMap)
    delete i.second;
}

bool UserManager::login(std::string username, std::string hashed_password) {

  int id = getUserIDByName(username);
  if (id == -1)
    return false;

  for (int i = 0; i < loggedUsers.size(); ++i)
    if (loggedUsers[i] == id)
      return false;

  User *user = userMap[id];
  if (user->getPassword() != hashed_password)
    return false;

  loggedUsers.push_back(id);

  return true;
}

bool UserManager::logout(std::string username) {

  int id = getUserIDByName(username);
  if (id == -1)
    return false;

  for (int i = 0; i < loggedUsers.size(); i++)
    if (loggedUsers[i] == id) {
      loggedUsers.erase(loggedUsers.begin() + i);
      return true;
    }

  return false;
}

bool UserManager::registerUser(std::string username, std::string raw_password) {

  int id = getUserIDByName(username);
  if (id == -1)
    return false;

  for (int i = 0; i < users.size(); i++)
    if (users[i] == id)
      return false;

  std::string hash = calculateSHA224(raw_password);
  usercnt++;
  userMap.insert(std::pair<int, User *>(usercnt, new User(usercnt, username, hash)));

  saveUserMap();
  // std::ofstream f;
  // f.open("userdata.csv", std::ios::app);
  // f << username << "," << hash << std::endl;
  // f.close();

  return true;
}

bool UserManager::isLogged(std::string username) {

  int id = getUserIDByName(username);
  if (id == -1)
    return false;

  for (int i = 0; i < loggedUsers.size(); i++)
    if (loggedUsers[i] == id)
      return true;

  return false;
}

User *UserManager::getUser(std::string username) {

  int id = getUserIDByName(username);
  if (id == -1)
    return nullptr;

  for (int i = 0; i < users.size(); i++)
    if (users[i] == id)
      return userMap[id];

  return nullptr;
}

std::vector<User *> UserManager::getLoggedUsers() {
  std::vector<User *> _loggedUsers;
  for (int i = 0; i < loggedUsers.size(); i++)
    _loggedUsers.push_back(userMap[loggedUsers[i]]);
  return _loggedUsers;
}

UserGroup *UserManager::getUserGroup(User *user) {
  int id = user->getID();
  if (id == -1)
    return nullptr;

  for (auto it = userGroupMap.begin(); it != userGroupMap.end(); ++it) {
    auto group = it->second;
    for (auto i : group->getUsers()) {
      if (i == id)
        return group;
    }
  }
  return nullptr;
}

int UserManager::getUserIDByName(std::string username) {
  for (auto i : userMap)
    if (i.second->getUsername() == username)
      return i.first;
  return -1;
}

void UserManager::saveUserMap() {
  Disk::blockid_t bid = 0;
  char *buf;
  for (auto i : userMap) {
    std::string str =
        std::to_string(i.second->getID()) + "," + i.second->getUsername() + "," + i.second->getPassword() + "\n";
    strcat(buf, str.c_str());
  }
  size_t buflen = Disk::PAGE_SIZ;
  ft->syswrite(bid, buf, buflen);

  // std::ofstream f;
  // f.open("userdata.csv", std::ios_base::trunc);
  // assert(f.is_open());
  // for (auto i : userMap) {
  //   f << i.second->getID() << "," << i.second->getUsername() << "," << i.second->getPassword() << std::endl;
  // }
  // f.close();
}

void UserManager::saveUserGroupMap() {
  Disk::blockid_t bid = 1;
  char *buf;
  for (auto i : userGroupMap) {
    std::string str = std::to_string(i.second->getId()) + "," + i.second->getName() + ",";
    str += std::to_string(i.second->getUsers().size());
    for (auto j : i.second->getUsers()) {
      str += "," + std::to_string(j);
    }
    str += "\n";
    strcat(buf, str.c_str());
  }
  size_t buflen = Disk::PAGE_SIZ;
  ft->syswrite(bid, buf, buflen);

  // std::ofstream f;
  // f.open("userGroupData.csv", std::ios_base::trunc);
  // assert(f.is_open());
  // for (auto i : userGroupMap) {
  //   f << i.second->getId() << "," << i.second->getName() << ',';
  //   f << i.second->getUsers().size();
  //   for (auto j : i.second->getUsers()) {
  //     f << "," << j;
  //   }
  //   f << std::endl;
  // }
  // f.close();
}