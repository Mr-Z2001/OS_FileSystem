#ifndef __USERFUNCTIONS_HPP__
#define __USERFUNCTIONS_HPP__

#include <string>
#include <vector>

void create(std::string filename, std::string username, int userPrivilege, std::string groupname, int groupPrivilege, std::string type);
void rm(std::string groupname, std::string username, bool force, bool interactive, bool recursive, bool directory, bool verbose, std::vector<std::string> filenames);
void open(std::string groupname, std::string username, std::string filename);
void close(std::string groupname, std::string username, std::string filename);
void read(std::string groupname, std::string username, std::string filename);
void cat(std::string groupname, std::string username, bool head, bool tail, bool more, bool number, int n, std::vector<std::string> filename);
void write(std::string groupname, std::string username, bool append, bool overwrite, std::string filename, std::string text);
void mkdir(std::string groupname, std::string username, bool m, bool parents, bool verbose, std::string directory);
void cd(std::string groupname, std::string username, std::string directory);
void pwd();
void ls(std::string groupname, std::string username, bool all, bool almost_all, bool _directory, bool human_readable, bool inode, bool list, bool recursive, bool sortBySize, bool sortByTime, std::string directory);
void mv(std::string groupname, std::string username, bool force, bool interactive, bool verbose, std::vector<std::string> source, std::string destination);
void cp(std::string groupname, std::string username, bool force, bool link, bool noclobber, bool update, bool verbose, std::vector<std::string> source, std::string destination);

#endif