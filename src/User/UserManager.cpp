#include "UserManager.hpp"
// #include "cryptopp/sha.h"
#include "utils.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>

void UserManager::loadUserMap() {
  std::ifstream f;
  f.open("userdata.csv");
  assert(f.is_open());
  std::string line;
  while (std::getline(f, line)) {
    std::vector<std::string> data;
    std::regex pattern(",");
    std::copy(std::sregex_token_iterator(line.begin(), line.end(), pattern, -1), std::sregex_token_iterator(),
              std::back_inserter(data));
    User *u = new User;
    u->setID(std::stod(data[0]));
    u->setUsername(data[1]);
    u->setPassword(data[2]);
    userMap.insert(std::pair<int, User *>(u->getID(), u));
    usercnt++;
  }
  f.close();
}

void UserManager::loadUserGroupMap() {
  std::ifstream f;
  f.open("userGroupData.csv");
  assert(f.is_open());
  std::string line;
  while (std::getline(f, line)) {
    std::vector<std::string> data;
    std::regex pattern(",");
    std::copy(std::sregex_token_iterator(line.begin(), line.end(), pattern, -1), std::sregex_token_iterator(),
              std::back_inserter(data));
    UserGroup *ug = new UserGroup;
    ug->setId(std::stod(data[0]));
    ug->setName(data[1]);
    for (int i = 2; i < data.size(); i++) {
      int u = std::stod(data[i]);
      ug->addUser(u);
    }
    userGroupMap.insert(std::pair<int, UserGroup *>(ug->getId(), ug));
  }
  f.close();
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
  if (~id)
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
  if (~id)
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
  if (~id)
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
  if (~id)
    return false;

  for (int i = 0; i < loggedUsers.size(); i++)
    if (loggedUsers[i] == id)
      return true;

  return false;
}

User *UserManager::getUser(std::string username) {

  int id = getUserIDByName(username);
  if (~id)
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
  if (~id)
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
  std::ofstream f;
  f.open("userdata.csv", std::ios_base::trunc);
  assert(f.is_open());
  for (auto i : userMap) {
    f << i.second->getID() << "," << i.second->getUsername() << "," << i.second->getPassword() << std::endl;
  }
  f.close();
}

void UserManager::saveUserGroupMap() {
  std::ofstream f;
  f.open("userGroupData.csv", std::ios_base::trunc);
  assert(f.is_open());
  for (auto i : userGroupMap) {
    f << i.second->getId() << "," << i.second->getName() << ',';
    f << i.second->getUsers().size();
    for (auto j : i.second->getUsers()) {
      f << "," << j;
    }
    f << std::endl;
  }
  f.close();
}