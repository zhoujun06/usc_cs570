CC=g++
CFLAGS=-c -g -Wall
OBJ=hw3

all : $(OBJ)

$(OBJ)	: main.o bst.o 
	$(CC)  main.o bst.o -o $(OBJ)

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

bst.o : bst.cpp 
	$(CC) $(CFLAGS) bst.cpp

clean:
	@rm -f *.o $(OBJ)
