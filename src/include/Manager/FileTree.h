//
// Created by 86158 on 2023/5/23.
//


#ifndef FILESYSTEM_FILETREE_H
#define FILESYSTEM_FILETREE_H
#include "Node.h"
#include "Identity.h"
class FileTree {

private:
    std::vector<Node*> nodes;
    Node* current_file;
    std::vector<std::string> split(std::string path,int type);//将路径切开，返回每个目录名，例如path=/A/B/C return A,B,C  type=0(绝对) type=1(相对)
    bool is_path(std::string &filename);//文件名是一个路径
    void delete_file(Node* file);                 //删除文件
    void delete_directory(Node* directory);       //删除目录
public:

    FileTree();
    void init();
    Node* find_node_by_name(std::string name);//按在当前目录下按文件名找到节点
    Node* find_node_by_path(std::string path);//按路径找到节点
    void create(Identity* id, std::string filename);
    void cwd(std::string cwd);
    void rm(Identity* id, bool force, bool interactive, bool recursive, bool directory, bool verbose,std::vector<std::string> filenames);
    void read(Identity* id,std::string filename);
    void cat(Identity id, bool head, bool tail, bool more, bool number, int n, std::vector<std::string> filename);
    void cd(std::string filename);//
    void print();


};


#endif //FILESYSTEM_FILETREE_H
