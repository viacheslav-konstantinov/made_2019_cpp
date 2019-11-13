CC=g++

all: serializer

run: serializer
	./serializer

serializer: main.o
	$(CC) -o serializer main.o

main.o: main.cpp
	$(CC) -c main.cppmake

test: serializer
	echo "Test 1 with correct deserialization"
	./serializer < input_test1.txt
	echo 
	echo "Test 2 with incorrect deserialization"
	./serializer < input_test2.txt

clean:
	rm -rf *.o serializer