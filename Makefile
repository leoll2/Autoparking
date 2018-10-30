CC = g++
CFLAGS = -Wall
ALLEG = -Iallegro5/ -lallegro -lallegro_primitives
CONF = -Iconf

.PHONY: all clean

all: main test_maneuver test_shapes test_collision test_movements

build/display.o: src/display.cpp src/display.h conf/display_params.h
	$(CC) $(CFLAGS) $(CONF) -c src/display.cpp -o build/display.o

build/main.o: src/main.cpp conf/field_params.h
	$(CC) $(CFLAGS) $(CONF) -c src/main.cpp -o build/main.o

build/maneuver.o: src/maneuver.cpp src/maneuver.h
	$(CC) $(CFLAGS) -c src/maneuver.cpp -o build/maneuver.o

build/map.o: src/map.cpp src/map.h conf/field_params.h
	$(CC) $(CFLAGS) $(CONF) -c src/map.cpp -o build/map.o
	
build/maps.o: src/maps.cpp src/maps.h conf/field_params.h
	$(CC) $(CFLAGS) $(CONF) -c src/maps.cpp -o build/maps.o

build/Q_learning_network.o: src/Q_learning_network.cpp src/Q_learning_network.h conf/AI_params.h conf/field_params.h
	$(CC) $(CFLAGS) $(CONF) -c src/Q_learning_network.cpp -o build/Q_learning_network.o

build/shapes.o: src/shapes.cpp src/shapes.h
	$(CC) $(CFLAGS) -c src/shapes.cpp -o build/shapes.o

build/test_maneuver.o: src/test_maneuver.cpp src/maneuver.h
	$(CC) $(CFLAGS) -c src/test_maneuver.cpp -o build/test_maneuver.o

build/test_shapes.o: src/test_shapes.cpp src/shapes.h
	$(CC) $(CFLAGS) -c src/test_shapes.cpp -o build/test_shapes.o
	
build/test_collision.o: src/test_collision.cpp
	$(CC) $(CFLAGS) $(CONF) -c src/test_collision.cpp -o build/test_collision.o
	
build/test_movements.o: src/test_movements.cpp
	$(CC) $(CFLAGS) $(CONF) -c src/test_movements.cpp -o build/test_movements.o

build/util.o: src/util.cpp src/util.h
	$(CC) $(CFLAGS) -c src/util.cpp -o build/util.o

build/vehicle.o: src/vehicle.cpp src/vehicle.h
	$(CC) $(CFLAGS) $(CONF) -c src/vehicle.cpp -o build/vehicle.o

main: build/main.o build/maneuver.o build/display.o build/vehicle.o build/map.o build/maps.o build/shapes.o build/Q_learning_network.o build/util.o
	$(CC) $(ALLEG)  build/main.o \
	build/display.o \
	build/maneuver.o \
	build/vehicle.o \
	build/map.o \
	build/maps.o \
	build/Q_learning_network.o \
	build/util.o \
	build/shapes.o -o bin/main

test_maneuver: build/maneuver.o build/test_maneuver.o build/util.o
	$(CC) build/maneuver.o build/test_maneuver.o build/util.o -o bin/test_maneuver
	
test_shapes: build/shapes.o build/test_shapes.o build/util.o
	$(CC) build/shapes.o build/test_shapes.o build/util.o -o bin/test_shapes
	
test_collision: build/test_collision.o build/maneuver.o build/display.o build/vehicle.o build/map.o build/Q_learning_network.o build/shapes.o build/util.o
	$(CC) $(ALLEG)  build/test_collision.o \
	build/display.o \
	build/maneuver.o \
	build/vehicle.o \
	build/map.o \
	build/Q_learning_network.o \
	build/util.o \
	build/shapes.o -o bin/test_collision

test_movements: build/test_movements.o build/maneuver.o build/display.o build/vehicle.o build/map.o build/maps.o build/Q_learning_network.o build/shapes.o build/util.o
	$(CC) $(ALLEG)  build/test_movements.o \
	build/display.o \
	build/maneuver.o \
	build/vehicle.o \
	build/map.o \
	build/maps.o \
	build/Q_learning_network.o \
	build/util.o \
	build/shapes.o -o bin/test_movements

clean:
	rm -f bin/*
	rm -f build/*
