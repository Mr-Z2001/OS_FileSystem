#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include "Shell.hpp"
#include "ShellFunctions.hpp"
#include "UserFunctions.hpp"
#include "utils.hpp"

Shell::Shell(char *username, char *hostname, char *cwd) {
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
  Identity *id;
  id->groupname = userGroup->getName();
  id->username = user->getUsername();
  while (true) {
    read_line();
    std::vector<std::string> tokens = parse(this->line_read);
    std::string command = tokens[0];
    int cmdType = getCommandType(command);
    switch (cmdType) {
    case 0: // create
    {
      std::string type = "file";
      for (auto i : tokens[1])
        if (i == '/')
          type = "dir";
      create(id, tokens[1], type);
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
      char *arg;
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
      std::vector<std::string> filenames;
      filenames.assign(tokens.begin() + optind, tokens.end());
      if (!_and && _xor)
        cat(id, h, t, m, n, cstrToInt(arg), filenames);
      else
        std::cerr << "Invalid combination of options" << std::endl;
    } break;
    case 4: // write

      break;
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
      char *arg;
      int opt;
      bool p = false, v = false, m = false;
      while ((opt = getopt(argc, argv, optstring)) != -1) {
        switch (opt) {
        case 'm':
          m = true;
          arg = optarg;
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
      mkdir(id, m, cstrToInt(optarg), p, v, tokens[optind]);
      break;
    } break;
    case 8: // cd
      cd(id, tokens[1], cwd);
      break;
    case 9: // pwd
      pwd(cwd);
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
    default: // unknown-command
      std::cerr << "Unknown command: " << command << std::endl;
      break;
    }
    free(this->line_read);
  }
}

void Shell::stop() { free(this->line_read); }

void Shell::read_line() {
  this->line_read = readline(this->prompt);
  if (!this->line_read)
    exit(1);
  if (this->line_read && *this->line_read)
    add_history(this->line_read);
}

void Shell::login() {
  std::string username;
  std::string password;
  std::string password_224;

  std::cout << "Username: ";
  std::cin >> username;
  password = getpass("Password: ");
  fflush(stdin);
  password_224 = calculateSHA224(password);

  bool state = userManager->login(username, password_224);
  if (state) {
    std::cout << "Welcome, " << username << '.' << std::endl;
    user = userManager->getUser(username);
    userGroup = userManager->getUserGroup(user);
  } else
    std::cout << "Login failed." << std::endl;
}

void Shell::logout() { userManager->logout(user->getUsername()); }