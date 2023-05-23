#include "Shell.hpp"
#include <stdlib.h>
#include <unistd.h>

Shell::Shell(char *username, char *hostname, char *cwd)
{
  this->username = username;
  this->hostname = hostname;
  this->cwd = cwd;
  this->prompt = (char *)malloc(1024);
  sprintf(this->prompt, "%s@%s:%s$ ", this->username, this->hostname, this->cwd);
}

Shell::~Shell()
{
  free(this->prompt);
}

char *Shell::getLineRead() { return this->line_read; }

char *Shell::getPrompt() { return this->prompt; }

void Shell::setLineRead(char *line_read) { this->line_read = line_read; }

void Shell::setPrompt(char *prompt) { this->prompt = prompt; }

void Shell::start()
{
  while (true)
  {
    read();
    // execute command
    free(this->line_read);
  }
}

void Shell::stop() { free(this->line_read); }

void Shell::read()
{
  this->line_read = readline(this->prompt);
  if (!this->line_read)
    exit(1);
  if (this->line_read && *this->line_read)
    add_history(this->line_read);
}