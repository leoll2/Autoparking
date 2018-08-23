#include "maps.h"

void SimpleParkingMap::initialize() {
    /* Add two rectangular obstacles */
    std::vector<Coordinate> ob1_coords = {	//must be counterclockwise
            {7*SPACE_UNIT,  18*SPACE_UNIT},
            {12*SPACE_UNIT, 18*SPACE_UNIT},
            {12*SPACE_UNIT, 24*SPACE_UNIT},
            {7*SPACE_UNIT,  24*SPACE_UNIT}};
    Polygon ob1(ob1_coords);
    this->add_obstacle(ob1);

    std::vector<Coordinate> ob2_coords = {	//must be counterclockwise
            {7*SPACE_UNIT,  0*SPACE_UNIT},
            {12*SPACE_UNIT, 0*SPACE_UNIT},
            {12*SPACE_UNIT, 6*SPACE_UNIT},
            {7*SPACE_UNIT,  6*SPACE_UNIT}};
    Polygon ob2(ob2_coords);
    this->add_obstacle(ob2);
}

SimpleParkingMap::SimpleParkingMap() : Map(HREF_POINTS, VREF_POINTS, 
                                           Coordinate(9*SPACE_UNIT, 8*SPACE_UNIT))
{
    initialize();
}