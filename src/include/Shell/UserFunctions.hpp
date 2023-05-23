#ifndef __USERFUNCTIONS_HPP__
#define __USERFUNCTIONS_HPP__

#include <string>
#include <vector>

void create(std::string filename);
void rm(bool force, bool interactive, bool recursive, bool directory, bool verbose, std::string filename);
void open(std::string filename);
void close(std::string filename);
void read(std::string filename);
void cat(bool head, bool tail, bool more, bool number, std::vector<std::string> filename);
void write(bool append, bool overwrite, std::string filename, std::string text);
void mkdir(bool parents, bool verbose, std::string directory);
void cd(std::string directory);
void pwd();
void ls(bool all, bool almost_all, bool author, bool _directory, bool human_readable, bool inode, bool list, bool recursive, bool sortBySize, bool sortByTime, std::string directory);
void mv(bool force, bool interactive, bool verbose, std::vector<std::string> source, std::string destination);
void cp(bool force, bool interactive, bool recursive, bool verbose, std::vector<std::string> source, std::string destination);

#endif