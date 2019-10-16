CC=g++

all: arithmeticParser

test: testParser
	./testParser --reporter compact --success

testParser:
	$(CC) -o testParser testParser.cpp

run: arithmeticParser
	./arithmeticParser

arithmeticParser: main.o
	$(CC) -o arithmeticParser main.o

main.o: main.cpp classArthParser.h
	$(CC) -c main.cpp

clean:
	rm -rf *.o arithmeticParser testParser


