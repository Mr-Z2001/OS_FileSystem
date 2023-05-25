#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "Shell.hpp"
#include "ShellFunctions.hpp"
#include "UserFunctions.hpp"

Shell::Shell(char *username, char *hostname, char *cwd)
{
  this->username = username;
  this->hostname = hostname;
  this->cwd = cwd;
  this->prompt = (char *)malloc(1024);
  sprintf(this->prompt, "%s@%s:%s$ ", this->username, this->hostname, this->cwd);
}

Shell::~Shell() { free(this->prompt); }

char *Shell::getLineRead() { return this->line_read; }

char *Shell::getPrompt() { return this->prompt; }

void Shell::setLineRead(char *line_read) { this->line_read = line_read; }

void Shell::setPrompt(char *prompt) { this->prompt = prompt; }

void Shell::loadCommandMap()
{
  std::ifstream f;
  f.open("CommandToInt.csv");
  std::string line;
  while (f >> line)
  {
    std::string command = line.substr(0, line.find(','));
    int cmdType = std::stoi(line.substr(line.find(',') + 1));
    this->commands[command] = cmdType;
  }
  f.close();
}

int Shell::getCommandType(std::string command)
{
  return this->commands[command];
}

void Shell::start()
{
  loadCommandMap();
  while (true)
  {
    read_line();
    std::vector<std::string> tokens = parse(this->line_read);
    std::string command = tokens[0];
    int cmdType = getCommandType(command);
    switch (cmdType)
    {
    case 0: // create
      create(tokens[1]);
      break;
    case 1: // open
      open(tokens[1]);
      break;
    case 2: // read
      read(tokens[1]);
      break;
    case 3: // cat
    {
      int argc = tokens.size();
      char *argv[argc];
      for (int i = 0; i < argc; ++i)
        argv[i] = const_cast<char *>(tokens[i].c_str());
      const char *optstring = "h::t::mn";
      int opt;
      bool h = false, t = false, m = false, n = false;
      char *arg;
      while ((opt = getopt(argc, argv, optstring)) != -1)
      {
        switch (opt)
        {
        case 'h':
          h = true, arg = optarg;
          break;
        case 't':
          t = true, arg = optarg;
          break;
        case 'm':
          m = true;
          break;
        case 'n':
          n = true;
          break;
        default:
          std::cerr << "Unknown option" << std::endl;
          break;
        }
      }
      // and = 0 && XOR = 1 (hmt)
      bool _and = h & t & m;
      bool _xor = h ^ t ^ m;
      if (!_and && _xor)
      {
        if (h)
          cat(true, false, false, arg, tokens);
        else if (t)
          cat(false, true, false, arg, tokens);
        else if (m)
          cat(false, false, true, false, tokens);
      }
      else
      {
        std::cerr << "Invalid combination of options" << std::endl;
      }
    }
    break;
    case 4: // write
      break;
    case 5: // close
      break;
    case 6: // close
      break;
    case 7: // mkdir
      break;
    case 8: // cd
      break;
    case 9: // pwd
      break;
    case 10: // ls
      break;
    case 11: // cp
      break;
    case 12: // mv
      break;
    default: // unknown-command
      std::cerr << "Unknown command: " << command << std::endl;
      break;
    }
    free(this->line_read);
  }
}

void Shell::stop() { free(this->line_read); }

void Shell::read_line()
{
  this->line_read = readline(this->prompt);
  if (!this->line_read)
    exit(1);
  if (this->line_read && *this->line_read)
    add_history(this->line_read);
}