#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <allegro5/allegro.h>	//never remove! (obscure compatibility reasons)
#include <allegro5/allegro_primitives.h>
#include "map.h"
#include "shapes.h"
#include "vehicle.h"

bool start_graphics(ALLEGRO_DISPLAY *display);

void display_all(const Map& m, const Vehicle& v); /*const Map& m, const Vehicle& v */

void end_graphics(ALLEGRO_DISPLAY *display);

#endif