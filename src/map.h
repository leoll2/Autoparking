#ifndef MAP_H
#define MAP_H

#include <vector>
#include "field_params.h"
#include "shapes.h"

/******************************************************************************/
/*              MAP: the field where the vehicle is moving                    */
/******************************************************************************/

class Map {
    public:
        const unsigned int width, height;
        std::vector<Polygon> obstacles;
        const Coordinate target;

        /* Adds an obstacle to the map. The shape is specified as Polygon. */
        void add_obstacle(Polygon& ob);
        /* Returns true if the coordinate is within the map boundaries. */
        bool is_within_boundaries(const Coordinate& c) const;
        /* Returns true if the polygon is completely inside the map. */
        bool is_within_boundaries(const Polygon& p) const;
        /* Returns true if the polygon collides with an obstacle. */
        bool collides_with_obstacles(const Polygon& p) const;
        /* Returns true if the arc collides with an obstacle. */
        bool collides_with_obstacles(const Arc& a) const;
        /* Constructor */
        Map(unsigned int w, unsigned int h, Coordinate t);
};


#endif