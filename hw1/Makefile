CC=g++
CFLAGS=-c -g -Wall
LD=/usr/ccs/bin/ld
LDFLAGS=-L/usr/usc/lib 
OBJ=hw1

all : $(OBJ)

$(OBJ)	: main.o utils.o mul.o div.o 
	$(CC)  main.o utils.o mul.o div.o -o $(OBJ)
	@#$(LD) $(LDFLAGS) main.o utils.o mul.o div.o -o $(OBJ)

main.o : main.cpp hw1_include.h
	$(CC) $(CFLAGS) main.cpp

mul.o : mul.cpp utils.cpp hw1_include.h
	$(CC) $(CFLAGS) mul.cpp

div.o : div.cpp utils.cpp hw1_include.h
	$(CC) $(CFLAGS) div.cpp

utils.o : utils.cpp hw1_include.h
	$(CC) $(CFLAGS) utils.cpp

clean:
	@rm -f *.o $(OBJ)
