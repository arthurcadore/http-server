all: 
	gcc ./src/server.c -o ./build/server 

run:
	./build/server