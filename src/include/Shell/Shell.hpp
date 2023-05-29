#ifndef __SHELL_HPP__
#define __SHELL_HPP__

#include <map>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>

#include "UserManager.hpp"
#include "FileTree.h"

class Shell {
private:
  char *line_read = (char *)NULL;
  // prompt=username@hostname:cwd$<space>
  char *prompt = (char *)NULL;
  char *username;
  char *hostname;
  char *cwd;

  User *user;
  UserGroup *userGroup;
  static UserManager *userManager;

  std::map<std::string, int> commands;

public:
  explicit Shell(char *username, char *hostname, char *cwd);
  ~Shell();

  char *getLineRead();
  char *getPrompt();

  void setLineRead(char *line_read);
  void setPrompt(char *prompt);

  void start();
  void stop();

  void read_line();
  void loadCommandMap();

  void login();
  void logout();

  User *getUser();

  static UserManager *getUserManager();

private:
  int getCommandType(std::string command);
};

#endif