CC=g++

all: tokenParser

run: tokenParser
	./tokenParser < input_run.txt

tokenParser: main.o
	$(CC) -o tokenParser main.o

main.o: main.cpp
	$(CC) -c main.cpp

test: tokenParser
	./tokenParser < input_test.txt

clean:
	rm -rf *.o tokenParser
