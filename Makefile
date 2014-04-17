BUILD_TYPE = Debug

all: build
	cd build; make

build: CMakeLists.txt
	mkdir -p build
	cd build; cmake -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) ..

run: all
	build/objectless

clean:
	rm -r build
