#include "UserManager.hpp"
// #include "cryptopp/sha.h"
#include "utils.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>

UserManager::UserManager() {
  std::ifstream f;
  f.open("userdata.csv");
  assert(f.is_open());
  std::string line;
  while (std::getline(f, line)) {
    std::vector<std::string> data;
    std::regex pattern(",");
    std::copy(std::sregex_token_iterator(line.begin(), line.end(), pattern, -1), std::sregex_token_iterator(),
              std::back_inserter(data));
    users.push_back(new User(std::stod(data[0]), data[1], data[2]));
  }
  f.close();
  f.open("userGroupData.csv");
  assert(f.is_open());
  while (std::getline(f, line)) {
    std::vector<std::string> data;
    std::regex pattern(",");
    std::copy(std::sregex_token_iterator(line.begin(), line.end(), pattern, -1), std::sregex_token_iterator(),
              std::back_inserter(data));
    UserGroup *ug;
    ug->setId(std::stod(data[0]));
    ug->setName(data[1]);
    for (int i = 2; i < data.size(); i++) {
      int u = std::stod(data[i]);
      ug->addUser(u);
    }
    userGroups.push_back(ug);
  }
  f.close();
}
UserManager::~UserManager() {
  for (int i = 0; i < users.size(); ++i)
    delete users[i];
  for (int i = 0; i < userGroups.size(); ++i)
    delete userGroups[i];
}

bool UserManager::login(std::string username, std::string password) {
  for (int i = 0; i < loggedUsers.size(); i++)
    if (loggedUsers[i]->getUsername() == username)
      return false;

  User *user = getUser(username);

  if (user == NULL)
    return false;

  std::string hash = calculateSHA224(password);
  if (user->getPassword() != hash)
    return false;

  loggedUsers.push_back(user);

  return true;
}

bool UserManager::logout(std::string username) {
  for (int i = 0; i < loggedUsers.size(); i++)
    if (loggedUsers[i]->getUsername() == username) {
      loggedUsers.erase(loggedUsers.begin() + i);
      return true;
    }

  return false;
}

bool UserManager::registerUser(std::string username, std::string password) {
  for (int i = 0; i < users.size(); i++)
    if (users[i]->getUsername() == username)
      return false;

  std::string hash = calculateSHA224(password);
  users.push_back(new User(users.size(), username, hash));

  std::ofstream f;
  f.open("userdata.csv", std::ios::app);
  f << username << "," << hash << std::endl;
  f.close();

  return true;
}

bool UserManager::isLogged(std::string username) {
  for (int i = 0; i < loggedUsers.size(); i++)
    if (loggedUsers[i]->getUsername() == username)
      return true;

  return false;
}

User *UserManager::getUser(std::string username) {
  for (int i = 0; i < users.size(); i++)
    if (users[i]->getUsername() == username)
      return users[i];

  return nullptr;
}

std::vector<User *> UserManager::getLoggedUsers() { return loggedUsers; }

UserGroup *UserManager::getUserGroup(User *user) {
  for (int i = 0; i < userGroups.size(); i++) {
    auto group = userGroups[i];
    auto groupUsers = group->getUsers();
    for (auto i : groupUsers)
      if (i == user->getID())
        return group;
  }
  return nullptr;
}