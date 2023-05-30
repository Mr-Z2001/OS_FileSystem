#include <iostream>
#include <string.h>
#include <unistd.h>

#include "Shell.hpp"

// init userManager
UserManager *Shell::userManager = Shell::getUserManager();

int main() {
  char username[256] = "user";
  char hostname[256];
  char *cwd = "/";
  gethostname(hostname, sizeof(hostname));
  Shell *shell = new Shell(username, hostname, cwd);
  shell->login();

  // while (shell->getUser() == nullptr) {
  // }
  shell->start();

  return 0;
}