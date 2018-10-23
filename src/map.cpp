#include "map.h"

/**
 * Check if a coordinate is within the boundaries of the map (borders included).
 * 
 * @param c the Coordinate
 * @return true if inside, false otherwise
 */
bool Map::is_within_boundaries(const Coordinate& c) const {
    return  c.x >= 0 &&
            c.x < SPACE_UNIT * width &&
            c.y >= 0 &&
            c.y < SPACE_UNIT * height;
}


/**
 * Add an obstacle object to the map.
 * 
 * @param ob the obstacle
 */
void Map::add_obstacle(Polygon& ob) {
    obstacles.push_back(ob);
}


/**
 * Check if a polygon is entirely within the boundaries of the map.
 * 
 * @param p the Polygon
 * @return true if inside, false otherwise
 */
bool Map::is_within_boundaries(const Polygon& p) const {
    std::vector<Coordinate> vertices = p.get_vertices1();
    for (auto v : vertices)
        if (!is_within_boundaries(v))
            return false;
    return true;
}


/**
 * Check if a polygon is colliding (overlaps) with any of the obstacles.
 * 
 * @param p the Polygon
 * @return true if colliding, false otherwise
 */
bool Map::collides_with_obstacles(const Polygon& p) const {
    for (auto o : obstacles)
        if (check_collision(o, p))
            return true;
    return false;
}


/**
 * Check if a circular arc is colliding (overlaps) with any of the obstacles.
 * 
 * @param a the Arc
 * @return true if colliding, false otherwise
 */
bool Map::collides_with_obstacles(const Arc& a) const {
    for (auto o : obstacles)
        if (check_collision(o, a))
            return true;
    return false;
}


/**
 * A Map represents the field on which the vehicle is moving.
 * 
 * @param w the width of the map
 * @param h the height of the map
 * @param t the target coordinate for parking
 */
Map::Map(unsigned int w, unsigned int h, Coordinate t) :
    width(w), 
    height(h),
    target(t)
{}
