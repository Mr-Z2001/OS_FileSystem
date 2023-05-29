//
// Created by 86158 on 2023/5/23.
//

#include "FileTree.h"

FileTree::FileTree()
{
    current_file= nullptr;
}

void FileTree::init()
{

    Node* root=new Node("/","directory");
    root->setIndex(0);
    Node* bin=new Node("bin","directory");
    bin->setIndex(1);
    Node* user=new Node("user","directory");
    user->setIndex(2);
    Node* home=new Node("home","directory");
    home->setIndex(3);
    current_file=root;
    root->addChild(bin);
    root->addChild(user);
    root->addChild(home);

    this->nodes.push_back(root);
    this->nodes.push_back(bin);
    this->nodes.push_back(user);
    this->nodes.push_back(home);

}
Node* FileTree:: find_node_by_name(std::string name) //在当前目录根据名字找到node
{
    if(current_file->getName()==name)
        return current_file;
    for(auto i:current_file->getChildren())
    {
        if(i.first==name)
            return i.second;
    }
    return nullptr;
}
bool FileTree::is_path(std::string &filename)
{
    int k=filename.find('/');
    if(k>=0)
        return true;
    return false;
}
std::vector<std::string> FileTree::split(std::string path,int type=0)//按/切割路径
{
    if(path[0]=='/')
        type=0;
    else type=1;
    std::vector<std::string>res;

    if(type==0) //绝对路径
    {
        int pos=path.find('/',1);
        auto word=path.substr(1,pos-1);
        path=path.substr(pos+1);
        res.push_back(word);
        if(pos<0)
            return res;
    }
    else    //相对路径
    {
        int pos=path.find('/');
        auto word=path.substr(0,pos);
        path=path.substr(pos+1);
        res.push_back(word);
        if(pos<0)
            return res;
    }

    while (path.size()>0)
    {
        int pos=path.find('/');
        if(pos<0)
            break;
        auto word=path.substr(0,pos);
        res.push_back(word);
        path=path.substr(pos+1);
    }
    res.push_back(path);
    return res;


}
Node* FileTree:: find_node_by_path(std::string path)//按路径查找
{
    std::vector<std::string> words;
    if(path[0]=='/') //绝对路径
    {
        current_file=nodes[0];
        words= split(path,0);//按绝对路径切片
    }
    else{
        words= split(path,1);//按相对路径切片
    }

    for (int i = 0; i < words.size(); ++i) {
        current_file=find_node_by_name(words[i]);
        if(current_file== nullptr)
        {
            return nullptr;
        }
    }
    return current_file;
}
void FileTree::create(Identity* id, std::string filename)
{
    if(!is_path(filename))
    {
        Node* file=new Node(id,filename);
        file->setIndex(nodes.size());
        current_file->addChild(file);
        this->nodes.push_back(file);
    }
    else
    {
        std::vector<std::string> words;
        if(filename[0]=='/') //绝对路径
        {
            current_file=nodes[0];
            words= split(filename,0);//按绝对路径切片
            for (int i = 0; i < words.size()-1; ++i) {
                current_file=find_node_by_name(words[i]);
                if(current_file== nullptr)
                {
                    std::cout<<"error path"<<std::endl;
                    exit(0);
                }
            }
            Node* newfile=new Node(id,words[words.size()-1]);
            newfile->setIndex(nodes.size());
            current_file->addChild(newfile);
            this->nodes.push_back(newfile);
        }

    }
}
void FileTree::cwd(std::string cwd)
{
    std::cout<<cwd<<std::endl;
}
void FileTree::delete_file(Node* file) //删除一个文件
{
    //blk_release(file->location); //释放硬盘空间
    auto parent=file->getParent();
    for (int i = 0; i < parent->children.size(); ++i) {
        if(parent->children[i]==file)
        {
            parent->children.erase(parent->children.begin()+i); //擦除父节点指针
        }
    }
    nodes[file->getIndex()]= nullptr; //对应位置置为空

    delete file;
}
void FileTree::delete_directory(Node* directory)
{
    if(directory->getType()=="directory")
    {
        if(directory->getChildren().size()==0)//空目录
        {
            auto parent=directory->getParent();
            for (int i = 0; i < parent->children.size(); ++i) {
                if(parent->children[i]==directory)
                {
                    parent->children.erase(parent->children.begin()+i); //擦除父节点指针
                }
            }
            nodes[directory->getIndex()]= nullptr; //对应位置置为空

            delete directory;
        }
        else //不是空目录
        {
            for (auto i:directory->getChildren()) {
                delete_directory(i.second);
            }
        }
    }

    if(directory->getType()=="file")
        delete_file(directory);

}
void FileTree::rm(Identity* id, bool force, bool interactive, bool recursive, bool directory, bool verbose,std::vector<std::string> filenames)
{
    auto temp_file=current_file;
    for(auto file:filenames)
    {
        current_file=temp_file;
        Node* file_pointer;
        if(is_path(file))
        {
            file_pointer=find_node_by_path(file);
        }
        else
        {
            file_pointer= find_node_by_name(file);
        }
        if(file_pointer== nullptr)//没找到文件
        {
            if(!force)
            {
                std::cout<<"error filename"<<std::endl;
                return;
            }
        }
        else //找到文件
        {
            if(file_pointer->getType()=="file") //是纯文件
            {
                std::string receiver="yes";
                if(interactive)
                {
                    std::cout<<"remove "<<file_pointer->name<<" ?(y/n)"<<std::endl;
                    std::cin>>receiver;
                }

                if(receiver[0]=='y')
                {
                    delete_file(file_pointer);
                    if(verbose)
                    {
                        std::cout<<"removing "<<file_pointer->name<<"......"<<std::endl;
                    }
                }


            }
            else if(file_pointer->getType()=="directory")//删除目录
            {
                if(recursive)
                {
                    delete_directory(file_pointer);
                }
            }
        }
    }
}
void FileTree::read(Identity* id,std::string filename)
{
    auto file=find_node_by_path(filename);
    if(file== nullptr)
    {
        std::cout<<"file no exist"<<std::endl;
        return;
    }
    else
    {
        auto gp=file->group_privilege;
        auto up=file->user_privilege;
        bool gp_permission= false;
        bool up_permission= false;
        for(auto i:gp)
        {
            if(i.first==id->groupname&&i.second>=100)
            {
                gp_permission= true;
            }
        }
        for(auto i:up)
        {
            if(i.first==id->username&&i.second>=100)
            {
                up_permission= true;
            }
        }
        if(gp_permission&&up_permission)
        {
            //blk_read(blockid_t bid, void *buf, size_t len);
        }
        else
        {
            std::cout<<"no permission"<<std::endl;
            return;
        }

    }
}
void FileTree::cat(Identity id, bool head, bool tail, bool more, bool number, int n, std::vector<std::string> filename) //1行32个字符，1块4096有128行
{
    if(head)
    {

    }
}
void FileTree::cd(std::string filename)
{
    current_file= find_node_by_path(filename);
}
void FileTree::print() {
    std::cout<<this->nodes.size()<<std::endl;
    auto root=nodes[0];
    auto children=root->getChildren();
    std::cout<<children.size()<<std::endl;
    for(auto i:children)
    {
        std::cout<<i.first<<" "<<i.second->getParent()->getName()<<std::endl;
    }
    std::cout<<current_file->getName()<<std::endl;

}