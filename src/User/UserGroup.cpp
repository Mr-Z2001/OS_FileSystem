#include "UserGroup.hpp"

UserGroup::UserGroup() {}
UserGroup::UserGroup(int id, std::string name) : id(id), name(name) {}
UserGroup::~UserGroup() {}

int UserGroup::getId() { return this->id; }

std::string UserGroup::getName() { return this->name; }

std::vector<User *> UserGroup::getUsers() { return this->users; }

void UserGroup::setId(int id) { this->id = id; }

void UserGroup::setName(std::string name) { this->name = name; }

void UserGroup::setUsers(std::vector<User *> users) { this->users = users; }

void UserGroup::addUser(User *user) { this->users.push_back(user); }

void UserGroup::removeUser(User *user)
{
  for (int i = 0; i < this->users.size(); i++)
  {
    if (this->users[i]->getUsername() == user->getUsername())
    {
      this->users.erase(this->users.begin() + i);
      break;
    }
  }
}
