#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <allegro5/allegro.h>	// do not remove! (obscure compatibility reasons)
#include <allegro5/allegro_primitives.h>
#include "map.h"
#include "Q_learning_network.h"
#include "shapes.h"
#include "vehicle.h"

bool start_graphics(ALLEGRO_DISPLAY *display);

void display_all(const Map& m, const Vehicle& v);

void display_rewards(const Q_LearningNetwork& ai);

void end_graphics(ALLEGRO_DISPLAY *display);

#endif