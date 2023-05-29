#include "UserGroup.hpp"

UserGroup::UserGroup() {}
UserGroup::UserGroup(int id, std::string name) : id(id), name(name) {}
UserGroup::~UserGroup() {}

int UserGroup::getId() { return this->id; }

std::string UserGroup::getName() { return this->name; }

std::vector<int> UserGroup::getUsers() { return this->users; }

void UserGroup::setId(int id) { this->id = id; }

void UserGroup::setName(std::string name) { this->name = name; }

void UserGroup::setUsers(std::vector<int> users) { this->users = users; }

void UserGroup::addUser(int userID) { this->users.push_back(userID); }

void UserGroup::removeUser(int user) {
  for (int i = 0; i < this->users.size(); i++) {
    if (this->users[i] == user) {
      this->users.erase(this->users.begin() + i);
      break;
    }
  }
}
