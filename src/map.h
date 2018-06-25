#ifndef MAP_H
#define MAP_H

#include <vector>
#include "shapes.h"

class Map {
	public:
		const unsigned int width,
			height;
		std::vector<Polygon> obstacles;
		
		void add_obstacle(Polygon& ob);
		Map(unsigned int w, unsigned int h);	
};


#endif