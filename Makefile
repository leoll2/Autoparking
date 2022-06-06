CC = g++
CFLAGS = -Wall
ALLEG = -Iallegro5/ -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf

BIN = bin
BUILD = build
CONF = conf
SRC = src
TEST = test

CONF_FILES = $(CONF)/AI_params.h $(CONF)/display_params.h $(CONF)/field_params.h

.PHONY: all clean

all: $(BIN)/main #$(BIN)/test_maneuver $(BIN)/test_shapes $(BIN)/test_movements $(BIN)/test_collision


# Object files

$(BUILD)/%.o: $(SRC)/%.cpp $(CONF_FILES)
	$(CC) $(CFLAGS) $(DEBUG) -I$(CONF) -c -o $@ $<


# Executables

$(BIN)/main: $(BUILD)/main.o $(BUILD)/maneuver.o $(BUILD)/display.o $(BUILD)/keyboard.o $(BUILD)/vehicle.o $(BUILD)/map.o $(BUILD)/maps.o $(BUILD)/shapes.o $(BUILD)/Q_learning_network.o $(BUILD)/util.o
	$(CC) -o $@ $^ $(ALLEG)

$(BIN)/test_maneuver: $(BUILD)/test_maneuver.o $(BUILD)/maneuver.o $(BUILD)/util.o
	$(CC) -o $@ $^ $(ALLEG)
	
$(BIN)/test_shapes: $(BUILD)/test_shapes.o $(BUILD)/shapes.o $(BUILD)/util.o
	$(CC) -o $@ $^ $(ALLEG)
	
$(BIN)/test_collision: $(BUILD)/test_collision.o $(BUILD)/maneuver.o $(BUILD)/display.o $(BUILD)/vehicle.o $(BUILD)/map.o $(BUILD)/Q_learning_network.o $(BUILD)/shapes.o $(BUILD)/util.o
	$(CC) -o $@ $^ $(ALLEG)

$(BIN)/test_movements: $(BUILD)/test_movements.o $(BUILD)/maneuver.o $(BUILD)/display.o $(BUILD)/vehicle.o $(BUILD)/map.o $(BUILD)/maps.o $(BUILD)/Q_learning_network.o $(BUILD)/shapes.o $(BUILD)/util.o
	$(CC) -o $@ $^ $(ALLEG)

clean:
	rm -f bin/*
	rm -f build/*
