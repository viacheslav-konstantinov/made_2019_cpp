CC=g++

all: ThreadPool test

run: ThreadPool
	./ThreadPool

ThreadPool: main.o ThreadPool.hpp
	$(CC) -pthread -o ThreadPool main.o

main.o: main.cpp ThreadPool.hpp
	$(CC) ${RUN_MODE} -c ThreadPool.hpp main.cpp
								
test: ThreadPool
	./ThreadPool < input.txt

clear:
	rm -rf *.o *.gch ThreadPool test
