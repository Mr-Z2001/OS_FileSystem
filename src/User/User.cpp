#include "User.hpp"

User::User() {}
User::User(int id, std::string username, std::string password) {
  this->id = id;
  this->username = username;
  this->password = password;
}

User::~User() {}

std::string User::getUsername() { return username; }

std::string User::getPassword() { return password; }

int User::getID() { return id; }

void User::setUsername(std::string username) { this->username = username; }

void User::setPassword(std::string password) { this->password = password; }

void User::setID(int id) { this->id = id; }
