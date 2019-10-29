CC=g++

all: linearAllocator

run: linearAllocator
	./linearAllocator

linearAllocator: main.o
	$(CC) -o linearAllocator main.o

main.o: main.cpp
	$(CC) -c main.cpp

test: testAllocator
	./testAllocator

testAllocator: testAllocator.cpp
	$(CC) -o testAllocator testAllocator.cpp

clean:
	rm -rf *.o linearAllocator testAllocator
