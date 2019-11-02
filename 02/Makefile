CC=g++

all: linearAllocator

run: linearAllocator
	./linearAllocator

linearAllocator: main.o
	$(CC) -o linearAllocator main.o

main.o: main.cpp
	$(CC) -c main.cpp

test: linearAllocator
	./linearAllocator

clean:
	rm -rf *.o linearAllocator
