#include "map.h"

bool Map::is_within_boundaries(const Coordinate& c) const {
	return 	c.x >= 0 &&
                c.x < SPACE_UNIT * width &&
                c.y >= 0 &&
                c.y < SPACE_UNIT * height;
}

void Map::add_obstacle(Polygon& ob) {
	obstacles.push_back(ob);
}

bool Map::is_within_boundaries(const Polygon& p) const {
	std::vector<Coordinate> vertices = p.get_vertices1();
	for (auto v : vertices)
		if (!is_within_boundaries(v))
			return false;
	return true;
}

bool Map::collides_with_obstacles(const Polygon& v) const {
	for (auto o : obstacles)
		if (check_collision(o, v))
			return true;
	return false;
}

Map::Map(unsigned int w, unsigned int h, Coordinate t) :
	width(w), 
        height(h),
        target(t)
{}