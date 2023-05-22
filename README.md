# OS_FileSystem

This is a repo of homework -- Operating System - File System -- project

---

# Environment
```bash
sudo apt update
sudo apt upgrade

# how to check if you have those
gcc --version
g++ --version
cmake --version

# install gcc, g++, cmake
sudo apt install build-essential
sudo apt install cmake

# you can install them by yourself by
sudo apt install gcc
sudo apt install g++
sudo apt install cmake
```

# How to build
```bash
cd OS_FileSystem
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

# How to run
```bash
cd build
cd test
./test
```