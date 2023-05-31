#ifndef __USERFUNCTIONS_HPP__
#define __USERFUNCTIONS_HPP__

#include "Identity.h"
#include <string>
#include <vector>

void init_ufs();

void create(Identity *identity, std::string filename); // deleted previleges, due to all those are 111.
void rm(Identity *identity, bool force, bool interactive, bool recursive, bool directory, bool verbose,
        std::vector<std::string> filenames);
void open(Identity *identity, std::string filename);
void close(Identity *identity, std::string filename);
void read(Identity *identity, std::string filename);
void cat(Identity *identity, bool head, bool tail, bool more, bool number, int n, std::vector<std::string> filename);
void write(Identity *identity, bool append, bool overwrite, std::string filename, std::string text);
void mkdir(Identity *identity, bool m, int usermode, int groupmode, bool parents, bool verbose, std::string directory);
void cd(Identity *identity, std::string directory, char *newDirectory);
void pwd(char *cwd);
void ls(Identity *identity, bool all, bool almost_all, bool _directory, bool human_readable, bool inode, bool list,
        bool recursive, bool sortBySize, bool sortByTime, std::string directory);
void mv(Identity *identity, bool force, bool interactive, bool verbose, std::vector<std::string> source,
        std::string destination);
void cp(Identity *identity, bool force, bool link, bool noclobber, bool update, bool verbose,
        std::vector<std::string> source, std::string destination);
void chmod(Identity *identity, bool recursive, int mode, std::vector<std::string> filename);

#endif