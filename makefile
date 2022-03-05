# all: main.cpp Person.cpp
# 	gcc -lstdc++ -std=c++11 -o main main.cpp Person.cpp -I.

#CC=gcc
CC=g++
# CFLAGS=-I.
# CFLAGS=-lstdc++ -std=c++11 -I.
CFLAGS= -std=c++11 -I.

all: main.cpp Person.cpp Problem.cpp
	$(CC) $(CFLAGS) -o main main.cpp Person.cpp Problem.cpp


# g++ -std=c++11 -I. -o main main.cpp Person.cpp Problem.cpp
