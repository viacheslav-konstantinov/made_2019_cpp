CC=g++

all: linearAllocator

test: testAllocator
	./testAllocator --reporter compact --success

testAllocator: 
	$(CC) -o testAllocator testAllocator.cpp

run: linearAllocator
	./linearAllocator

linearAllocator: main.o
	$(CC) -o linearAllocator main.o

main.o: main.cpp linearAllocator.h
	$(CC) -c main.cpp

clean:
	rm -rf *.o linearAllocator testAllocator
