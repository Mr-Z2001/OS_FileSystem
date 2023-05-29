//
// Created by 86158 on 2023/5/23.
//

#include "Node.h"
Node::Node() {

}
Node::Node(std::string name)
{
    this->name=name;
}
Node::Node(std::string name,std::string type) {
    this->name=name;
    this->type=type;
}
Node::Node(Identity* id,std::string filename)
{
    this->name=filename;
    this->group_privilege.insert({id->groupname,111});
    this->user_privilege.insert({id->username,111});
    this->type="file";

}

void Node::addChild(Node*& child)
{
    this->children.push_back(child);
    child->parent=this;
}
std::vector<std::pair<std::string,Node*>> Node::getChildren()
{
    std::vector<std::pair<std::string,Node*>> res;
    for(auto i:children)
    {
        res.push_back({i->name,i});
    }
    return res;
}
Node* Node::getParent()
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
void Node::setIndex(int i)
{
    index=i;
}
int Node::getIndex()
{
    return index;
}
