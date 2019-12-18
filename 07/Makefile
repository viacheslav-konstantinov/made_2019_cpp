CC=g++

all: pythonString test1 test2 test3 test4

run: pythonString
	./pythonString

pythonString: main.o
	$(CC) -o pythonString main.o

main.o: main.cpp
	$(CC) -c pythonString.hpp main.cpp


test: test1 test2 test3 test4
	./test1;	\
	./test2;	\
	./test3;	\
	./test4;

test1: test1Norm.o
	$(CC) -o test1 test1Norm.o
test1Norm.o: test1Norm.cpp
	$(CC) -c pythonString.hpp test1Norm.cpp

test2: test2ArgIdxOutOfRange.o
	$(CC) -o test2 test2ArgIdxOutOfRange.o
test2ArgIdxOutOfRange.o: test2ArgIdxOutOfRange.cpp
	$(CC) -c pythonString.hpp test2ArgIdxOutOfRange.cpp

test3: test3BraceOutOfContext.o
	$(CC) -o test3 test3BraceOutOfContext.o
test3BraceOutOfContext.o: test3BraceOutOfContext.cpp
	$(CC) -c pythonString.hpp test3BraceOutOfContext.cpp

test4: test4NotIntegerInBraces.o
	$(CC) -o test4 test4NotIntegerInBraces.o
test4NotIntegerInBraces.o: test4NotIntegerInBraces.cpp
	$(CC) -c pythonString.hpp test4NotIntegerInBraces.cpp

clean:
	rm -rf *.o *.gch pythonString test1 test2 test3 test4