#include <allegro5/allegro.h>	//never remove! (obscure compatibility reasons)
#include <chrono>
#include <cstdio>
#include <iostream>
#include <thread>
#include "display.h"
#include "field_params.h"
#include "maps.h"
#include "shapes.h"
#include "vehicle.h"
#include "vehicle_params.h"
#include "maps.h"

/* Mapping between keys and maneuvers. */
unsigned int get_maneuver_code(int c) {
    switch(c) {
        case 'q':
            return 20;
        case 'w':
            return 17;
        case 'e':
            return 23;
        case 'r':
            return 29;
        case 't':
            return 26;
        case 'a':
            return 19;
        case 's':
            return 16;
        case 'd':
            return 22;
        case 'f':
            return 28;
        case 'g':
            return 25;
        case 'z':
            return 18;
        case 'x':
            return 15;
        case 'c':
            return 21;
        case 'v':
            return 27;
        case 'b':
            return 24;
        case 'Q':
            return 9;
        case 'W':
            return 12;
        case 'E':
            return 6;
        case 'R':
            return 0;
        case 'T':
            return 3;
        case 'A':
            return 10;
        case 'S':
            return 13;
        case 'D':
            return 7;
        case 'F':
            return 1;
        case 'G':
            return 4;
        case 'Z':
            return 11;
        case 'X':
            return 14;
        case 'C':
            return 8;
        case 'V':
            return 2;
        case 'B':
            return 5;
        default:
            return 30;
    }
}

int main(int argc, char **argv) {
	
    ALLEGRO_DISPLAY *display = NULL;

    if(!start_graphics(display))
        return -1;

    SimpleParkingMap map;

    Vehicle car = Vehicle(CAR_LENGTH, CAR_WIDTH, Coordinate(4*SPACE_UNIT, 8*SPACE_UNIT), pi/2);
    display_all_entities(map, car);

    int c;
    unsigned int ret;
    std::cout << "Insert characters to perform maneuvers. Write . to exit.\n";
    do {
        c = getchar();
        if (c == '\n')
            continue;
        unsigned int mnv_code = get_maneuver_code(c);
        if (mnv_code >= 30)
            std::cout << "Invalid key" << std::endl;
        else {
            Vehicle ghost(car);
            Maneuver mnv(mnv_code);
            ret = car.move(map, mnv);
            std::cout << ret << std::endl;
            display_all_entities_enhanced(map, ghost, car, 10, 50);
        }
    } while (c != '.');

    end_graphics(display);

    return 0;
}