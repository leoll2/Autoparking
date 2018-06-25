#include "map.h"

void Map::add_obstacle(Polygon& ob) {
	obstacles.push_back(ob);
}

Map::Map(unsigned int w, unsigned int h) :
	width(w), height(h) {}