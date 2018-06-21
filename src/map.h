#ifndef MAP_H
#define MAP_H

#include <vector>
#include "obstacle.h"

using namespace std;

class Map {
	public:
		const unsigned int width,
			height;
		vector<Polygon> obstacles;
		
		void add_obstacle(Polygon& ob);
		Map(unsigned int w, unsigned int h);
	
}


#endif MAP_H