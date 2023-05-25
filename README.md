# OS_FileSystem

This is a repo of homework -- Operating System - File System -- project

---

# Environment
```bash
sudo apt update
sudo apt upgrade

# check if you have those
gcc --version
g++ --version
cmake --version

# install gcc, g++, cmake, readline
sudo apt install build-essential
sudo apt install cmake
sudo apt install libreadline-dev
sudo apt install libssl-dev
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
# if run unit test
cd <folder>
./test
# else run main
./main
```

# How to add test
1. cd to certain folder, for example test/User
2. add a new cpp file, for example UserTest.cpp
3. if you need extra header files only for testing, add them to test/include
4. add the following code to CMakeLists.txt
```cmake
add_executable(target_name src0.cpp src1.cpp ...)
target_link_libraries(target_name PUBLIC library_name)
```
>target_name is the name of the executable file, you can name it whatever you want, for example, UserTest
  
>src0.cpp... those are source codes for compiling. for example, UserTest.cpp

>library_name is the name of the library you want to link. for example, User(you can find libs in src/CMakeLists.txt)
