CC = g++
CFLAGS = -Wall
INC = -Iinclude/
ALLEG = -Iallegro5/

.PHONY: all clean

all: test_maneuver

build/test_maneuver.o: src/test_maneuver.cpp src/maneuver.h src/test_maneuver.cpp
	$(CC) $(CFLAGS) $(INC) -c src/test_maneuver.cpp -o build/test_maneuver.o

build/maneuver.o: src/maneuver.cpp src/maneuver.h src/maneuver.cpp
	$(CC) $(CFLAGS) $(INC) -c src/maneuver.cpp -o build/maneuver.o


test_maneuver: build/maneuver.o build/test_maneuver.o
	$(CC) build/maneuver.o build/test_maneuver.o -o bin/test_maneuver

clean:
	rm -f bin/*
	rm -f build/*
