CC=g++

all: vectorClass test1 test2

run: vectorClass
	./vectorClass

vectorClass: main.o
	$(CC) -o vectorClass main.o

main.o: main.cpp
	$(CC) -c Allocator.hpp Iterator.hpp Vector.hpp main.cpp

test: test1 test2
	./test1; \
	./test2; \

test1: test1.o
	$(CC) -o test1 test1.o

test1.o: test1.cpp
	$(CC) -c Allocator.hpp Iterator.hpp Vector.hpp test1.cpp

test2: test2.o
	$(CC) -o test2 test2.o

test2.o: test2.cpp
	$(CC) -c Allocator.hpp Iterator.hpp Vector.hpp test2.cpp

clear:
	rm -rf *.o *.gch vectorClass test1 test2
