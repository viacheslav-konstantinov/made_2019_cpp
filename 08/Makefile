CC=g++

all: vectorClass test1 test2

run: vectorClass
	./vectorClass

vectorClass: main.o
	$(CC) -o vectorClass main.o

main.o: main.cpp Allocator.hpp Iterator.hpp Vector.hpp
	$(CC) -c main.cpp Allocator.hpp Iterator.hpp Vector.hpp

test: test1 test2
	./test1; \
	./test2; \

test1: test1.o
	$(CC) -o test1 test1.o

test1.o: test1.cpp Allocator.hpp Iterator.hpp Vector.hpp
	$(CC) -c test1.cpp Allocator.hpp Iterator.hpp Vector.hpp

test2: test2.o
	$(CC) -o test2 test2.o 

test2.o: test2.cpp Allocator.hpp Iterator.hpp Vector.hpp
	$(CC) -c test2.cpp Allocator.hpp Iterator.hpp Vector.hpp

clear:
	rm -rf *.o *.gch vectorClass test1 test2
