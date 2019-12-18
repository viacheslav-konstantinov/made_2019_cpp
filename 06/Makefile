CC=g++

all: bigInteger

run: bigInteger
	./bigInteger

bigInteger: main.o
	$(CC) -o bigInteger main.o

main.o: main.cpp
	$(CC) -c main.cpp

test: bigInteger
	./bigInteger < input.txt

clean:
	rm -rf *.o bigInteger