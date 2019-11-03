CC=g++

all: tockenParser

run: tockenParser
	./tockenParser < input_run.txt

tockenParser: main.o
	$(CC) -o tockenParser main.o

main.o: main.cpp
	$(CC) -c main.cpp

test: tockenParser
	./tockenParser < input_test.txt

clean:
	rm -rf *.o tockenParser