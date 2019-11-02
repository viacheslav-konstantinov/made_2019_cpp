CC=g++

all: arithmeticParser

test: testParser
	./testParser --reporter compact --success

testParser: arithmeticParser
	$(CC) -o testParser testParser.cpp

run: arithmeticParser
	./arithmeticParser 4+9*2+2*-3

arithmeticParser: main.o
	$(CC) -o arithmeticParser main.o

main.o: main.cpp
	$(CC) -c main.cpp

clean:
	rm -rf *.o arithmeticParser testParser
