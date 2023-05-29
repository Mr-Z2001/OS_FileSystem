//
// Created by 86158 on 2023/5/23.
//

#include "Node.h"
Node::Node() {

}
Node::Node(std::string name)
{
    fd=1;
    this->name=name;
}
Node::Node(std::string name,std::string type) {
    fd=1;
    this->name=name;
    this->type=type;
}
Node::Node(Identity* id,std::string filename)
{
    fd=1;
    this->name=filename;
    this->group_privilege.insert({id->groupname,111});
    this->user_privilege.insert({id->username,111});
    this->type="file";

}
Node::Node(Identity* id,int user_mode,int group_mode,std::string filename)
{
    fd=1;
    this->name=filename;
    this->group_privilege.insert({id->groupname,group_mode});
    this->user_privilege.insert({id->username,user_mode});
    this->type="directory";
}

int Node::getParent()
{
    return parent;
}
std::string Node::getName()
{
    return this->name;
}
std::string Node::getType()
{
    return this,type;
}

