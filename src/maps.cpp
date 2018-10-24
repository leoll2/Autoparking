#include "maps.h"

/******************************************************************************/
/*          SIMPLE PARKING MAP: a typical parallel parking scenario           */
/******************************************************************************/

/**
 * Initialize the map adding obstacles.
 */
void SimpleParkingMap::initialize() {

    // Add first obstacle
    std::vector<Coordinate> ob1_coords = {	//must be counterclockwise
            {7*SPACE_UNIT,  18*SPACE_UNIT},
            {12*SPACE_UNIT, 18*SPACE_UNIT},
            {12*SPACE_UNIT, 24*SPACE_UNIT},
            {7*SPACE_UNIT,  24*SPACE_UNIT}};
    Polygon ob1(ob1_coords);
    this->add_obstacle(ob1);

    // Add second obstacle
    std::vector<Coordinate> ob2_coords = {	//must be counterclockwise
            {7*SPACE_UNIT,  0*SPACE_UNIT},
            {12*SPACE_UNIT, 0*SPACE_UNIT},
            {12*SPACE_UNIT, 6*SPACE_UNIT},
            {7*SPACE_UNIT,  6*SPACE_UNIT}};
    Polygon ob2(ob2_coords);
    this->add_obstacle(ob2);
}

/**
 * A typical parallel parking map.
 */
SimpleParkingMap::SimpleParkingMap() : 
        Map(HREF_POINTS, VREF_POINTS, Coordinate(9*SPACE_UNIT, 8*SPACE_UNIT)) {
    initialize();
}


/******************************************************************************/
/*             TWISTED PARKING MAP: a zig-zag parking scenario                */
/******************************************************************************/

/**
 * Initialize the map adding obstacles.
 */
void TwistedParkingMap::initialize() {
    
    /* Add the first obstacle */
    std::vector<Coordinate> ob1_coords = {	//must be counterclockwise
            {0*SPACE_UNIT,  16*SPACE_UNIT},
            {5*SPACE_UNIT,  16*SPACE_UNIT},
            {5*SPACE_UNIT,  24*SPACE_UNIT},
            {0*SPACE_UNIT,  24*SPACE_UNIT}};
    Polygon ob1(ob1_coords);
    this->add_obstacle(ob1);

    // Add the second obstacle
    std::vector<Coordinate> ob2_coords = {	//must be counterclockwise
            {7*SPACE_UNIT,  0*SPACE_UNIT},
            {12*SPACE_UNIT, 0*SPACE_UNIT},
            {12*SPACE_UNIT, 6*SPACE_UNIT},
            {7*SPACE_UNIT,  6*SPACE_UNIT}};
    Polygon ob2(ob2_coords);
    this->add_obstacle(ob2);
}


/**
 * A zig-zag parking map.
 */
TwistedParkingMap::TwistedParkingMap() : 
        Map(HREF_POINTS, VREF_POINTS, Coordinate(8*SPACE_UNIT, 14*SPACE_UNIT)) {
    initialize();
}
