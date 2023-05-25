#ifndef __SHELL_HPP__
#define __SHELL_HPP__

#include <readline/readline.h>
#include <readline/history.h>
#include <map>
#include <string>

#include "UserManager.hpp"

class Shell
{
private:
  char *line_read = (char *)NULL;
  // prompt=username@hostname:cwd$<space>
  char *prompt = (char *)NULL;
  char *username;
  char *hostname;
  char *cwd;

  static UserManager *userManager;
  User *user;
  UserGroup *userGroup;

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

private:
  int getCommandType(std::string command);
};

#endif