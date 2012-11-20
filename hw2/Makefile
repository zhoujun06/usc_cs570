CC=g++
CFLAGS=-c -g -Wall
OBJ=hw2

all : $(OBJ)

$(OBJ)	: main.o trans.o disp.o my570list.o 
	$(CC)  main.o trans.o disp.o my570list.o -o $(OBJ)

main.o : main.cpp
	$(CC) $(CFLAGS) main.cpp

my570list.o : my570list.cpp 
	$(CC) $(CFLAGS) my570list.cpp

trans.o : trans.cpp my570list.cpp 
	$(CC) $(CFLAGS) trans.cpp

disp.o : disp.cpp my570list.cpp 
	$(CC) $(CFLAGS) disp.cpp

clean:
	@rm -f *.o $(OBJ)
