CC=g++

all: matrix

run: matrix
	./matrix

matrix: main.o
	$(CC) -o matrix main.o

main.o: main.cpp
	$(CC) -c main.cpp

test: matrix
	./matrix < input_test.txt

clean:
	rm -rf *.o matrix