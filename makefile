
# macbook pro
CC=clang++
CFLAGS= -std=c++11 -I.

# windows
# CC=g++
# CFLAGS= -std=c++11 -I.

all: main.cpp Person.cpp Problem.cpp
	$(CC) $(CFLAGS) -o main main.cpp Person.cpp Problem.cpp


# g++ -std=c++11 -I. -o main main.cpp Person.cpp Problem.cpp
