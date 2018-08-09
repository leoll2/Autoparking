#ifndef MAP_H
#define MAP_H

#include <vector>
#include "field_params.h"
#include "shapes.h"

class Map {
	private:
		bool is_within_boundaries(const Coordinate& c) const;
	
	public:
		const unsigned int width,
			height;
		std::vector<Polygon> obstacles;
		
		void add_obstacle(Polygon& ob);
		bool is_within_boundaries(const Polygon& v) const;
		bool collides_with_obstacles(const Polygon& v) const;
		Map(unsigned int w, unsigned int h);
};


#endif