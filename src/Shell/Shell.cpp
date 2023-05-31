#include <cassert>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "Shell.hpp"
#include "ShellFunctions.hpp"
#include "UserFunctions.hpp"
#include "utils.hpp"

Shell::Shell(char *username, char *hostname, char *cwd) {
  init_ufs();
  this->username = username;
  this->hostname = hostname;
  strncpy(this->cwd, cwd, 128);
  this->prompt = (char *)malloc(1024);
  this->userManager = new UserManager;
  sprintf(this->prompt, "%s@%s:%s$ ", this->username, this->hostname, this->cwd);
}

Shell::~Shell() {
  delete user;
  delete userGroup;
  delete userManager;
  // free(this->prompt);
}

char *Shell::getLineRead() { return this->line_read; }

char *Shell::getPrompt() {
  pwd(cwd);
  sprintf(this->prompt, "%s@%s:%s$ ", this->username, this->hostname, cwd);
  return this->prompt;
}

void Shell::setLineRead(char *line_read) { this->line_read = line_read; }

void Shell::setPrompt(char *prompt) { this->prompt = prompt; }

UserManager *Shell::getUserManager() {
  if (Shell::userManager == nullptr)
    Shell::userManager = new UserManager();
  return Shell::userManager;
}

void Shell::loadCommandMap() {
  std::ifstream f;
  f.open("CommandToInt.csv");
  std::string line;
  while (f >> line) {
    std::string command = line.substr(0, line.find(','));
    int cmdType = std::stoi(line.substr(line.find(',') + 1));
    this->commands[command] = cmdType;
  }
  f.close();
}

int Shell::getCommandType(std::string command) { return this->commands[command]; }

void Shell::start() {
  loadCommandMap();
  Identity id[1];
  id->groupname = userGroup->getName();
  id->username = user->getUsername();
  bool end_flag = false;
  while (!end_flag) {
    read_line();
    optind = 0;
    std::vector<std::string> tokens = parse(this->line_read);
    std::string command = tokens[0];
    int cmdType = getCommandType(command);
    switch (cmdType) {
    case 0: // create
    {
      create(id, tokens[1]);
      break;
    }
    case 1: // open
      open(id, tokens[1]);
      break;
    case 2: // read
      read(id, tokens[1]);
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
      char *arg = NULL;
      while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
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
      bool none = !h & !t & !m;
      std::vector<std::string> filenames;
      filenames.assign(tokens.begin() + optind, tokens.end());
      if (!_and && _xor || none)
        cat(id, h, t, m, n, cstrToInt(arg), filenames);
      else
        std::cerr << "Invalid combination of options" << std::endl;
    } break;
    case 4: // write
    {
      int argc = tokens.size();
      char *argv[argc];
      for (int i = 0; i < argc; ++i)
        argv[i] = const_cast<char *>(tokens[i].c_str());
      const char *optstring = "ao";
      int opt;
      bool a = false, o = false;
      while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
        case 'a':
          a = true;
          break;
        case 'o':
          o = true;
          break;
        default:
          std::cerr << "Unknown option" << std::endl;
          break;
        }
      }
      std::string filename;
      std::string text;
      filename = tokens[optind];
      text = tokens[optind + 1];
      write(id, a, o, filename, text);
      break;
    }
    case 5: // close
      close(id, tokens[1]);
      break;
    case 6: // rm
    {
      int argc = tokens.size();
      char *argv[argc];
      for (int i = 0; i < argc; ++i)
        argv[i] = const_cast<char *>(tokens[i].c_str());
      const char *optstring = "firRdv";
      int opt;
      bool f = false, i = false, r = false, d = false, v = false;
      while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
        case 'f':
          f = true;
          break;
        case 'i':
          i = true;
          break;
        case 'r':
        case 'R':
          r = true;
          break;
        case 'd':
          d = true;
          break;
        case 'v':
          v = true;
          break;
        default:
          std::cerr << "Unknown option" << std::endl;
          break;
        }
      }
      std::vector<std::string> filenames;
      filenames.assign(tokens.begin() + optind, tokens.end());
      rm(id, f, i, r, d, v, filenames);
      break;
    } break;
    case 7: // mkdir
    {
      int argc = tokens.size();
      char *argv[argc];
      for (int i = 0; i < argc; ++i)
        argv[i] = const_cast<char *>(tokens[i].c_str());
      const char *optstring = "m::pv";
      char *arg1 = NULL, *arg2 = NULL;
      int opt;
      bool p = false, v = false, m = false;
      while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
        case 'm':
          m = true;
          arg1 = optarg;
          arg2 = optarg;
        case 'p':
          p = true;
          break;
        case 'v':
          v = true;
          break;
        default:
          std::cerr << "Unknown option" << std::endl;
          break;
        }
      }
      mkdir(id, m, cstrToInt(arg1), cstrToInt(arg2), p, v, tokens[optind]);
      break;
    }
    case 8: // cd
      if (tokens.size() == 1)
        tokens.push_back("");
      if (tokens[1] == "~" || tokens[1] == "")
        tokens[1] = "/home/" + id->username;
      cd(id, tokens[1], cwd);
      break;
    case 9: // pwd
      pwd(cwd);
      std::cout << cwd << std::endl;
      break;
    case 10: // ls
    {
      int argc = tokens.size();
      char *argv[argc];
      for (int i = 0; i < argc; ++i)
        argv[i] = const_cast<char *>(tokens[i].c_str());
      const char *optstring = "aAdhilRrSt";
      int opt;
      bool a = false, A = false, d = false, h = false, i = false, l = false, R = false, S = false, t = false;
      while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
        case 'a':
          a = true;
          break;
        case 'A':
          A = true;
          break;
        case 'd':
          d = true;
          break;
        case 'h':
          h = true;
          break;
        case 'i':
          i = true;
          break;
        case 'l':
          l = true;
          break;
        case 'R':
        case 'r':
          R = true;
          break;
        case 'S':
          S = true;
          break;
        case 't':
          t = true;
          break;
        default:
          std::cerr << "Unknown option" << std::endl;
          break;
        }
      }
      ls(id, a, A, d, h, i, l, R, S, t, tokens[optind]);
      break;
    }
    case 11: // cp
    {
      int argc = tokens.size();
      char *argv[argc];
      for (int i = 0; i < argc; ++i)
        argv[i] = const_cast<char *>(tokens[i].c_str());
      const char *optstring = "flnuv";
      int opt;
      bool f = false, l = false, n = false, u = false, v = false;
      while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
        case 'f':
          f = true;
          break;
        case 'l':
          l = true;
          break;
        case 'n':
          n = true;
          break;
        case 'u':
          u = true;
          break;
        case 'v':
          v = true;
          break;
        default:
          std::cerr << "Unknown option" << std::endl;
          break;
        }
      }
      std::vector<std::string> filenames;
      filenames.assign(tokens.begin() + optind, tokens.end() - 1);
      cp(id, f, l, n, u, v, filenames, tokens.back());
      break;
    }
    case 12: // mv
    {
      int argc = tokens.size();
      char *argv[argc];
      for (int i = 0; i < argc; ++i)
        argv[i] = const_cast<char *>(tokens[i].c_str());
      const char *optstring = "fiv";
      int opt;
      bool f = false, i = false, v = false;
      while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
        case 'f':
          f = true;
          break;
        case 'i':
          i = true;
          break;
        case 'v':
          v = true;
          break;
        default:
          std::cerr << "Unknown option" << std::endl;
          break;
        }
      }
      std::vector<std::string> filenames;
      filenames.assign(tokens.begin() + optind, tokens.end() - 1);
      mv(id, f, i, v, filenames, tokens.back());
      break;
    }
    case 13: // exit
    {
      std::cout << "Bye!" << std::endl;
      end_flag = true;
      break;
    }
    default: // unknown-command
      std::cerr << "Unknown command: " << command << std::endl;
      break;
    }
    free(this->line_read);
  }
}

void Shell::stop() { free(this->line_read); }

void Shell::read_line() {
  this->line_read = readline(getPrompt());
  if (!this->line_read)
    exit(1);
  if (this->line_read && *this->line_read)
    add_history(this->line_read);
}

void Shell::login() {
  std::string _username;
  std::string _password;
  char pwd[128];
  std::string _password_224;

  std::cout << "Username: ";

  std::cin >> _username;
  // std::cin.ignore();
  std::cout << "Password: ";
  getpass(pwd);
  _password = pwd;
  // _password = getpass("Password: ");
  // fflush(stdin);
  // _username = "admin";
  // _password = "password";
  _password_224 = calculateSHA224(_password);

  bool state = userManager->login(_username, _password_224);
  if (state) {
    std::cout << "Welcome, " << _username << '.' << std::endl;
    user = userManager->getUser(_username);
    userGroup = userManager->getUserGroup(user);
    strcpy(this->username, _username.c_str());
  } else {
    std::cout << "Login failed." << std::endl;
    assert(0);
  }
}

void Shell::logout() { userManager->logout(user->getUsername()); }

User *Shell::getUser() { return user; }