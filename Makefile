all: main Makefile
	./main
main: Makefile src/*.cpp src/*.h
	g++ -std=c++11 -Os  -o main -I./src src/*.cpp -lGL -lSDL2 -lGLEW
	# this will drop some k. keep it for cutting stage
	# this might also be a good starting point for custom libc: https://github.com/ensc/dietlibc
	# g++ -std=c++11 -Os  -fno-exceptions -fno-rtti -o main -I./src src/*.cpp -lGL -lGLU -lSDL2
	
