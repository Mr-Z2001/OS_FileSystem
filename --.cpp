#include <iostream>
#include <string>
#include <vector>

using namespace std;

class File {
private:
  string name;
  string content;

public:
  File(string name) : name(name), content("") {}

  string getName() { return name; }

  string getContent() { return content; }

  void setContent(string c) { content = c; }
};

class Directory {
public:
  string name;
  vector<File *> files;
  vector<Directory *> subDirectories;

public:
  Directory(string name) : name(name) {}

  string getName() { return name; }

  void addFile(File *file) { files.push_back(file); }

  void addSubDirectory(Directory *directory) {
    subDirectories.push_back(directory);
  }

  vector<string> getFileNames() {
    vector<string> names;
    for (int i = 0; i < files.size(); i++) {
      names.push_back(files[i]->getName());
    }
    return names;
  }

  vector<string> getSubDirectoryNames() {
    vector<string> names;
    for (int i = 0; i < subDirectories.size(); i++) {
      names.push_back(subDirectories[i]->getName());
    }
    return names;
  }

  File *getFile(string name) {
    for (int i = 0; i < files.size(); i++) {
      if (files[i]->getName() == name) {
        return files[i];
      }
    }
    return NULL;
  }

  Directory *getSubDirectory(string name) {
    for (int i = 0; i < subDirectories.size(); i++) {
      if (subDirectories[i]->getName() == name) {
        return subDirectories[i];
      }
    }
    return NULL;
  }
};

class FileSystem {
private:
  Directory *root;

public:
  FileSystem() { root = new Directory("root"); }

  void createFile(string path) {
    int lastSlash = path.find_last_of("/");
    string dirPath = path.substr(0, lastSlash);
    string name = path.substr(lastSlash + 1);

    Directory *dir = navigateTo(dirPath);
    if (dir != NULL) {
      File *file = new File(name);
      dir->addFile(file);
    }
  }

  void createDirectory(string path) {
    int lastSlash = path.find_last_of("/");
    string dirPath = path.substr(0, lastSlash);
    string name = path.substr(lastSlash + 1);

    Directory *dir = navigateTo(dirPath);
    if (dir != NULL) {
      Directory *subDir = new Directory(name);
      dir->addSubDirectory(subDir);
    }
  }

  void deleteFile(string path) {
    int lastSlash = path.find_last_of("/");
    string dirPath = path.substr(0, lastSlash);
    string name = path.substr(lastSlash + 1);

    Directory *dir = navigateTo(dirPath);
    if (dir != NULL) {
      for (int i = 0; i < dir->getFileNames().size(); i++) {
        if (dir->getFileNames()[i] == name) {
          dir->files.erase(dir->files.begin() + i);
          return;
        }
      }
    }
  }

  void deleteDirectory(string path) {
    int lastSlash = path.find_last_of("/");
    string dirPath = path.substr(0, lastSlash);
    string name = path.substr(lastSlash + 1);

    Directory *dir = navigateTo(dirPath);
    if (dir != NULL) {
      for (int i = 0; i < dir->getSubDirectoryNames().size(); i++) {
        if (dir->getSubDirectoryNames()[i] == name) {
          dir->subDirectories.erase(dir->subDirectories.begin() + i);
          return;
        }
      }
    }
  }

  void writeFile(string path, string content) {
    int lastSlash = path.find_last_of("/");
    string dirPath = path.substr(0, lastSlash);
    string name = path.substr(lastSlash + 1);

    Directory *dir = navigateTo(dirPath);
    if (dir != NULL) {
      for (int i = 0; i < dir->getFileNames().size(); i++) {
        if (dir->getFileNames()[i] == name) {
          dir->files[i]->setContent(content);
          return;
        }
      }
    }
  }

  string readFile(string path) {
    int lastSlash = path.find_last_of("/");
    string dirPath = path.substr(0, lastSlash);
    string name = path.substr(lastSlash + 1);

    Directory *dir = navigateTo(dirPath);
    if (dir != NULL) {
      for (int i = 0; i < dir->getFileNames().size(); i++) {
        if (dir->getFileNames()[i] == name) {
          return dir->files[i]->getContent();
        }
      }
    }
    return "";
  }

  Directory *navigateTo(string path) {
    if (path == "") {
      return root;
    }

    int lastSlash = path.find_last_of("/");
    string dirPath = path.substr(0, lastSlash);
    string name = path.substr(lastSlash + 1);

    Directory *dir = navigateTo(dirPath);
    if (dir != NULL) {
      for (int i = 0; i < dir->getSubDirectoryNames().size(); i++) {
        if (dir->getSubDirectoryNames()[i] == name) {
          return dir->subDirectories[i];
        }
      }
    }
    return NULL;
  }

  void printDirectory(Directory *dir, int level) {
    for (int i = 0; i < level; i++) {
      cout << "  ";
    }
    cout << dir->getName() << endl;

    for (int i = 0; i < dir->getFileNames().size(); i++) {
      for (int j = 0; j < level + 1; j++) {
        cout << "  ";
      }
      cout << dir->getFileNames()[i] << endl;
    }

    for (int i = 0; i < dir->getSubDirectoryNames().size(); i++) {
      printDirectory(dir->subDirectories[i], level + 1);
    }
  }

  void printDirectory(string path) {
    Directory *dir = navigateTo(path);
    if (dir != NULL) {
      printDirectory(dir, 0);
    }
  }

  void printDirectory() { printDirectory(root, 0); }
};
