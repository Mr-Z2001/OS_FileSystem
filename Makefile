ALLSRC	:= $(shell find . -name *.cpp)

all: $(ALLSRC)
	-rm -rf build
	-mkdir -p build
	cd build && cmake ..
	cmake --build build -j8

exec:
	cd ./build/test && gdb ./main