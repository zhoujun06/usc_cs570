CC=g++
CFLAGS=-c -g -Wall
OBJ=hw4

all : $(OBJ)

$(OBJ)	: my570list.o maze.o mazedata.o problem.o search.o main.o 
	$(CC) my570list.o maze.o mazedata.o problem.o search.o main.o -o $(OBJ)

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

my570list.o : my570list.cpp 
	$(CC) $(CFLAGS) my570list.cpp

mazedata.o : mazedata.cpp mazedata.hpp
	$(CC) $(CFLAGS) mazedata.cpp

search.o : search.cpp queue.hpp search.hpp
	$(CC) $(CFLAGS) search.cpp

problem.o : problem.cpp problem.hpp
	$(CC) $(CFLAGS) problem.cpp

maze.o : maze.cpp maze.hpp
	$(CC) $(CFLAGS) maze.cpp

clean:
	@rm -f *.o $(OBJ)
