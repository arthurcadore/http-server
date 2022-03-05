all: buildDir
	gcc server.c -o ./build/server 

buildDir:
	mkdir build

run:
	./build/server