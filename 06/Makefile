CC=g++

all: bigInteger

run: bigInteger
	./bigInteger

bigInteger: main.o bigInteger.o
	$(CC) -o bigInteger bigInteger.o main.o

main.o: main.cpp
	$(CC) -c main.cpp

bigInteger.o: bigInteger.cpp
	$(CC) -c bigInteger.hpp bigInteger.cpp

test: bigInteger
	./bigInteger < input.txt

clean:
	rm -rf *.o *.gch bigInteger
