#include "Shell.hpp"
#include <iostream>
#include <string.h>
#include <unistd.h>

// init userManager
UserManager *Shell::userManager = Shell::getUserManager();

int main() {
  char username[256] = "user";
  char hostname[256];
  char *cwd = "~";
  gethostname(hostname, sizeof(hostname));
  Shell *shell = new Shell(username, hostname, cwd);

  while (shell->getUser() == nullptr) {
    shell->login();
  }
  shell->start();

  return 0;
}