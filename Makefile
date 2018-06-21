CC = g++
CFLAGS = -Wall
INC = -Iinclude/
ALLEG = -Iallegro5/

.PHONY: all clean

all: test_maneuver test_shapes

build/maneuver.o: src/maneuver.cpp src/maneuver.h
	$(CC) $(CFLAGS) $(INC) -c src/maneuver.cpp -o build/maneuver.o

build/shapes.o: src/shapes.cpp src/shapes.h
	$(CC) $(CFLAGS) $(INC) -c src/shapes.cpp -o build/shapes.o

build/util.o: src/util.cpp src/util.h
	$(CC) $(CFLAGS) $(INC) -c src/util.cpp -o build/util.o

build/test_maneuver.o: src/test_maneuver.cpp src/maneuver.h
	$(CC) $(CFLAGS) $(INC) -c src/test_maneuver.cpp -o build/test_maneuver.o

build/test_shapes.o: src/test_shapes.cpp src/shapes.h
	$(CC) $(CFLAGS) $(INC) -c src/test_shapes.cpp -o build/test_shapes.o

test_maneuver: build/maneuver.o build/test_maneuver.o build/util.o
	$(CC) build/maneuver.o build/test_maneuver.o build/util.o -o bin/test_maneuver
	
test_shapes: build/shapes.o build/test_shapes.o build/util.o
	$(CC) build/shapes.o build/test_shapes.o build/util.o -o bin/test_shapes

clean:
	rm -f bin/*
	rm -f build/*
