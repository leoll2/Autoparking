#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "map.h"
#include "Q_learning_network.h"
#include "shapes.h"
#include "vehicle.h"

bool start_graphics(ALLEGRO_DISPLAY *display);

void display_all_entities(const Map& m, const Q_LearningNetwork& ai, const Vehicle& v);

void display_all_entities_enhanced(const Map& map, const Q_LearningNetwork& ai, const Vehicle& car_start, 
        const Vehicle& car_end, unsigned int frames, unsigned int millis = 100);

void display_rewards(const Q_LearningNetwork& ai);

void end_graphics(ALLEGRO_DISPLAY *display);

#endif